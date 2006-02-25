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

#ifndef DIR_H
#define DIR_H

#include <string>

inline std::string strip_ext(const std::string& filename){
    std::string::size_type dot_pos = filename.find_last_of('.');
    return (std::string::npos == dot_pos) ? filename : filename.substr(0, dot_pos);
}

inline std::string directory(const std::string& path) {
    std::string::size_type pos = path.find_last_of('/');
    return (pos == std::string::npos) ? std::string(".") : path.substr(0,pos);
}

std::string get_cwd();

#include <unistd.h> //chdir
#include <errno.h>
#include <utils/syserr.h>

inline void change_dir(const std::string& dir) {
    if(chdir(dir.c_str()) == -1)
        throw system_error("chdir",errno);
}

#include <libgen.h> //dirname

inline std::string change_dir_file(const std::string& path){
    char buf[path.size()+1];
    strcpy(buf,path.c_str());
    std::string cwd = get_cwd();
    change_dir(dirname(buf));
    return cwd;
}


#endif//DIR_H

