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
#include "session_manager.h"
#include <HttpSessionImpl.h>
#include <apr_shm.h>
#include "syserr.h"
#include <assert.h>
#include <shared_repository.h>

using namespace std;

#define SHM_SIZE 1024


//----------------------------------------------------------------------------
// session_manager::session_manager(){
// }
//----------------------------------------------------------------------------
session_manager& session_manager::instance() {
    static session_manager instance;
    return instance;
}
//----------------------------------------------------------------------------
static string mangle_id(const string& id){
    static const char CHARS[62] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
        'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };
    string mangled;
    mangled.reserve(id.size());
    for(unsigned i=0; i < id.size(); ++i)
        mangled += CHARS[ abs(static_cast<int>(id[i]%sizeof(CHARS))) ];
    return mangled;
}
//----------------------------------------------------------------------------
string session_manager::generate_session_id(request_rec *r) const{
    string id;

    //full request time (including msec)
    char buf[sizeof(r->request_time)+1];
    memcpy(buf, &r->request_time, sizeof(r->request_time));
    buf[sizeof(r->request_time)] = '\0';
    id=buf;

    //port
    apr_port_t port;
    apr_sockaddr_port_get(&port, r->connection->remote_addr);
    memcpy(buf, &port, sizeof(apr_port_t));
    buf[sizeof(int)] = '\0';
    id+=buf;

    //ip
    string::size_type start=0,end;
    string ip = r->connection->remote_ip;
    for(unsigned i=0;i<4;++i){
        end = ip.find('.', start);
        id += (char)atoi(ip.substr(start,end-start).c_str());
        start = end+1;
    }

    //random
    const int size = 8;
    unsigned char randbytes[size];
    apr_generate_random_bytes(randbytes, size);
    id.append(reinterpret_cast<char*>(randbytes), 8);

    return mangle_id(id);
}

HttpSession* session_manager::get(const string& id){
    string val = shared_repository::instance().get(id);
    if(val.empty())
        return NULL;
    long creation = reinterpret_cast<long>(val.data());
    long accessed = reinterpret_cast<long>(val.data() + sizeof(long));
    return new HttpSessionImpl(id, creation, accessed);
}
//----------------------------------------------------------------------------
HttpSession* session_manager::create(request_rec *r){
    string id = generate_session_id(r);
    HttpSessionImpl* session = new HttpSessionImpl(id);
    long times[] = {
        session->getCreationTime(),
        session->getLastAccessedTime()
    };
    string val(reinterpret_cast<char*>(&times), 2*sizeof(long));
    shared_repository::instance().set(id, val);
    pending.push_front(id);
    return session;
}
//----------------------------------------------------------------------------
void session_manager::commit(HttpServletResponse& resp) {
    while(!pending.empty()) {
        resp.addCookie(Cookie("JSESSIONID", pending.front()));
        pending.pop_front();
    }
}
