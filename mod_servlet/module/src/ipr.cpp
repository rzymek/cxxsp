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
#include "ipr.h"
#include <vector>
#include <boost/serialization/vector.hpp>
#include <iostream>
using namespace std;

#include <assert.h>

#include <fstream>

//~ #include <boost/lexical_cast.hpp>
//~ #include <sys/timeb.h>
//~ using boost::lexical_cast;

//~ ostream& operator<<(ostream& out, const struct timeb& tb) {
    //~ return out << tb.time << '.' << tb.millitm;
//~ }

//===========================================================
   
ipr::server::server(const string& addr_path) 
    //: mutex(boost::read_write_scheduling_policy::alternating_many_reads)
{
    net::unix_socket sock;
    sock.bind(addr_path.c_str());
    sock.listen(5);
    main_loop(sock);
}

pid_t ipr::server::dispatch(const string& addr_path) {
    pid_t pid = fork();
    if(pid == -1) 
        throw system_error("fork", errno);
    else if(pid == 0) { //child
        server the_server(addr_path);
        exit(1);
    }else{ //parent
        return pid;
    }
}

void ipr::server::main_loop(net::unix_socket& sock) {
    for(;;) {
        int sockfd = sock.accept();        
        //~ ofstream log("/tmp/dbg/ipr.log", ios_base::app);
        //~ log << "creating new thread" << endl;
        //~ log.close();
        boost::thread thrd(
            boost::bind(
                &server::process, 
                boost::ref(*this),
                sockfd
            )
        );
    }
}

void ipr::server::process(int sockfd) {
    //~ string logname = "/tmp/dbg/ipr_"+lexical_cast<string>(pthread_self())+".log";
    //~ struct timeb tb;
    //~ {
        //~ ofstream log("/tmp/dbg/ipr.log", ios_base::app);
        //~ log << "New thread:" << pthread_self() << ". Logname is " << logname << endl;
        //~ log.close();
    //~ }
    
    ofstream log("/home/rzymek/devel/00cxxsp/profile/servlet.log", ios_base::app);
    log << "ipr::server::process: " << getpid() << ", thread_id: " << pthread_self() << endl;
    log.close();   
    
    fdstream stream(sockfd);
    operation_type type;
    repository *rep;
    {
        boost::mutex::scoped_lock lock(repositories_mutex);
        rep = &repositories[""];
    }
    while(stream) {
        //these 2 need to be inside the loop. otherwize 'data[id] = value' saves refeneces and doesn't copy the values
        string id,value;
        {
            iarchive ia(stream, boost::archive::no_header);
            ia >> type;
            ia >> id;
            switch(type) {
                case OT_SET:  
                    ia >> value;
                    {
                        //~ ofstream log(logname.c_str(), ios_base::app);
                        //~ ftime(&tb);
                        //~ log << "OT_SET rep['" << rep->id << "'] has '" << id << "': " << 
                            //~ ( (rep->data.find(id) == rep->data.end())?"NO":"YES" ) 
                            //~ << " setting: " << value << " at " << tb << endl;
                        boost::mutex::scoped_lock lock(*rep->mutex);
                        //boost::read_write_mutex::scoped_write_lock lock(mutex);                        
                        rep->data[id] = value;
                        //~ ftime(&tb);
                        //~ log << "value rep['" << rep->id << "'] with id='" << id << "' is set to " << 
                            //~ rep->data[id] << " at " << tb << endl;
                        //~ log.close();
                    }
                    break;
                case OT_OPEN: 
                    {
                        boost::mutex::scoped_lock lock(repositories_mutex);
                        //~ ofstream log(logname.c_str(), ios_base::app);
                        //~ log << "OT_OPEN: repositories has '" << id << "': " << 
                            //~ ( (repositories.find(id) == repositories.end())?"NO":"YES" )
                            //~ << endl;                        
                        rep = &repositories[id];
                        rep->id = id;
                        //~ log.close();
                    }
                    break;
                case OT_REMOVE:
                    {
                        boost::mutex::scoped_lock lock(*rep->mutex);
                        //boost::read_write_mutex::scoped_write_lock lock(mutex);                        
                        rep->data.erase(id);
                    }
                case OT_GET: //nothing more to 
                {
                        //~ ofstream log(logname.c_str(), ios_base::app);
                        //~ ftime(&tb);
                        //~ log << "OT_GET rep['" << rep->id << "'] value of '" << id << "': " << ": '" << 
                            //~ ((rep->data.find(id) == rep->data.end()) ? "NULL" : rep->data[id]) << "'" << 
                            //~ " at " << tb << endl;
                        //~ log.close();
                }
                case OT_LIST:
                case OT_HAS:
                    break;
            }
        }
        {
            oarchive oa(stream, boost::archive::no_header);
            switch(type) {
                case OT_GET: {
                    {
                        boost::mutex::scoped_lock lock(*rep->mutex);
                        //boost::read_write_mutex::scoped_read_lock lock(mutex);
                        //Mayby move sending out of scoped_lock? 
                        map_type::const_iterator entry = rep->data.find(id);
                        if(entry == rep->data.end()) {
                            string empty;
                            oa << empty;
                        }else
                            oa << entry->second;
                    }
                    break;
                }
                case OT_HAS: {
                    bool has_it;
                    {
                        boost::mutex::scoped_lock lock(*rep->mutex);
                        //boost::read_write_mutex::scoped_read_lock lock(mutex);
                        has_it = (rep->data.find(id) != rep->data.end());
                    }
                    oa << has_it;
                    break;
                }
                case OT_LIST: {
                    vector<string> list;
                    {
                        boost::mutex::scoped_lock lock(*rep->mutex);
                        //boost::read_write_mutex::scoped_read_lock lock(mutex);
                        list.reserve(rep->data.size());
                        for(map_type::const_iterator iter=rep->data.begin();
                            iter != rep->data.end(); ++iter)
                        {
                            list.push_back(iter->first);
                        }                        
                    }
                    //TODO: oa << list;
                }
                case OT_REMOVE: //nothing to send
                case OT_OPEN:
                case OT_SET: 
                    {
                        bool ok = true;
                        oa << ok;
                    }
                    break;

            }                
        }
    }    
}

//====================================================================

ipr::client::client(const string& addr_path) 
    : stream(sock.connect(addr_path))
{ 
}

void ipr::client::sync() {
    //~ stream.flush();
    //~ if(fdatasync(sock.fd()) == -1)
        //~ throw system_error("fdatasync",errno);
}

void ipr::client::open(const string& rep) {
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_OPEN;
        oa << type << rep;
    }{
        //read empty response
        iarchive ia(stream, boost::archive::no_header); 
        bool isok = false;
        ia >> isok;
        assert(isok);
    }
    //~ sync();
}

bool ipr::client::has(const string& id) {
    //see: http://boost.org/libs/serialization/doc/exceptions.html#stream_error
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_HAS;
        oa << type << id;
    }{
        iarchive ia(stream, boost::archive::no_header);
        bool has_it;
        ia >> has_it;
        return has_it;
    }
    //~ sync();
}

auto_ptr< vector<string> > ipr::client::get_names() {
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_LIST;
        oa << type;
    }{
        iarchive ia(stream, boost::archive::no_header);
        auto_ptr< vector<string> > list(new vector<string>());
        ia >> *list;
        return list;
    }
}

void ipr::client::remove(const string& name) {
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_REMOVE;
        oa << type << name;
    }{
        //read empty response
        iarchive ia(stream, boost::archive::no_header);        
        bool isok = false;
        ia >> isok;
        assert(isok);
    }
    //~ sync();
}
//=====================================================================

string ipr::client::get(const string& id) {
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_GET;
        oa << type << id;
    }
    string value;
    {
        iarchive ia(stream, boost::archive::no_header);
        ia >> value;
    }
    //~ sync();
    return value;
}


void ipr::client::set(const string& id, const string& value) {
    //see: http://boost.org/libs/serialization/doc/exceptions.html#stream_error
    {
        oarchive oa(stream, boost::archive::no_header);
        const operation_type type = OT_SET;
        string str_val = value;
        oa << type << id << str_val;
    }{
        //read empty response
        iarchive ia(stream, boost::archive::no_header); 
        bool isok = false;
        ia >> isok;
        assert(isok);
    }
    //~ sync();
}
