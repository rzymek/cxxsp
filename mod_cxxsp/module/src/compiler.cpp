/*
 *  Copyright (C) 2005 Krzysztof Rzymkowski <rzymek@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "compiler.h"
#include <utils/syserr.h>
#include <boost/scoped_array.hpp>

#include <iostream>
#include <fstream>

//#define REPORT_COMPILER_EXIT_STATUS

using namespace std;
using boost::scoped_array;

namespace cxxsp {
    
    class compiler_error : public std::runtime_error {
    public:
        compiler_error(const std::string& msg) : std::runtime_error(msg) 
            { }
    };

    #define STDOUT 1
    #define STDERR 2

    static string read_all(int fd){
        string result;

        const size_t BUF_SIZE = 512;
        char buf[BUF_SIZE];
        ssize_t count;
        for(;;){
            count = read(fd,buf,BUF_SIZE);
            if(count < 0) {
                if(errno == EINTR)
                    continue;
                else
                    throw system_error("read", errno);
            }
            if(count == 0)
                break;
            result.append(buf, count);
        }
        return result;
    }

    char** make_c_array(const vector<string>& args) {
        char** argv = new char*[args.size()+1];
        for(unsigned i=0; i<args.size(); ++i) {
            argv[i] = new char[args[i].size()+1];
            strcpy(argv[i], args[i].c_str());
        }
        argv[args.size()] = NULL;
        return argv;
    }
    
    void setup_descriptors(int* cmd_stdout, int* cmd_stderr) {
        /* Close normal STDOUT & STDERR - we'll be setting then in a momemt */
        close(STDOUT);
        close(STDERR);
        /* Set STDOUT to cmd_stdout[1] */
        dup2(cmd_stdout[1], STDOUT);
        /* Set STDERR to cmd_stderr[1] */
        dup2(cmd_stderr[1], STDERR);

        /* We've only needed cmd_stderr[1] & cmd_stdout[1]
        for assigning the to STDERR & STDOUT, so now
        we dont need any of these descriptors */
        close(cmd_stdout[0]);
        close(cmd_stdout[1]);
        close(cmd_stderr[0]);
        close(cmd_stderr[1]);
    }
    void compiler::compiler_main(const string& cpp_path, const string& out_path, const vector<string>& params) {
        /*
        Remove .so file to avoid overwriting, which causes problems when
        unloading servlets. Reason: before unloading a servlet the destroy methon needs to
        be called. If the servlet gets overwritten, then call'ing it's method causes a SEGV

        Don't check the return value. The file may not exist - this is not an error.
        In case of other errors - they will be reported later (presumably by the compiler, when trying to overwrite)
        */
        unlink(out_path.c_str());
        
        static const char* static_argv[] = {
            "/usr/bin/g++",
            "-shared",
            "-fPIC",
            "-o"
        };
        static const size_t static_argv_size = 4;
        
        vector<string> args;
        args.reserve(
            sizeof(static_argv) +
            2 +
            2*includes_.size() + 
            params.size()
        );
        
        for(unsigned i=0; i<static_argv_size;++i)
            args.push_back(static_argv[i]);
        args.push_back(out_path);
        args.push_back(cpp_path);
        for(unsigned i=0; i<includes_.size(); ++i) {
            args.push_back("-I");
            args.push_back(includes_[i]);
        }
        for(unsigned i=0; i<default_flags_.size(); ++i)
            args.push_back(default_flags_[i]);
        for(unsigned i=0; i<params.size(); ++i)
            args.push_back(params[i]);
        
        ofstream err("/tmp/cxxsp.log");
        err << "\nThe full command is:\n\t"; 
        for(unsigned i=1; i<args.size(); ++i)
            err << args[i] << ' ';
        err << endl;
        err.close();

        /*
        Note: No need for memory management.
        In a momemt this process will exit and all the memory will be freed
        */
        char** argv = make_c_array(args);
        if(execv(static_argv[0], argv) == -1) {
            perror("Failed to execute the compiler");
            cerr << "\nThe full command is:\n\t"; 
            for(unsigned i=1; i<args.size(); ++i)
                cerr << args[i] << ' ';
            cerr << endl;
            exit(111);
        }
        exit(222);
    }

    void parent(pid_t pid, int* cmd_stdout, int* cmd_stderr) {
        /* We dont need input descriptors, we'll be only reading from
        the child */
        close(cmd_stderr[1]);
        close(cmd_stdout[1]);

        string buf = read_all(cmd_stderr[0]);
        buf += read_all(cmd_stdout[0]);
        /* We've read it all, not needed any more */
        close(cmd_stderr[0]);
        close(cmd_stdout[0]);


        /* Clean up child's status */
        int status;
        if(waitpid(pid, &status, 0/*options*/) == -1)
            throw system_error("waitpid", errno);
#ifdef REPORT_COMPILER_EXIT_STATUS
        /* Compiler process reported errors - throw exception */
        stringstream exit_info;
        if(WIFSIGNALED(status)) {
            exit_info << "Compiler exited due to signal "<<
                WTERMSIG(status) << endl;
        }else if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
            exit_info << "Compiler exited normaly with status " <<
                WEXITSTATUS(status) << endl;
        }
        buf.insert(0, exit_info.str() + "\n");
#endif
        if(!buf.empty()) {
            throw compiler_error(buf);
        }
    }
    
    void compiler::process(const string& cpp_path, const string& out_path, const vector<string>& params) {
        int cmd_stdout[2];//0-read, 1-write;
        int cmd_stderr[2];//0-read, 1-write;
        if(pipe(cmd_stdout) == -1 || pipe(cmd_stderr) == -1)
                throw system_error("pipe",errno);
        pid_t pid = fork();
        switch(pid){
            case -1:
                throw system_error("fork", errno);
            case 0: //child
                setup_descriptors(cmd_stdout, cmd_stderr);
                compiler_main(cpp_path, out_path, params);            
                break;
            default:
                parent(pid, cmd_stdout, cmd_stderr);
        }
    }

}
