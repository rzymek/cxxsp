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
#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string>

#include <utils/syserr.h>

namespace net {
    class unix_socket {
        int sockfd;
    public:
        inline unix_socket();
        inline ~unix_socket();
        void bind(const std::string& sun_path);        
        inline void listen(int backlog) ;
        int accept();  
        int connect(const std::string& sun_path);
        int fd() 
            { return sockfd; }
    };
    
    
    unix_socket::unix_socket() {
        sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
        if(sockfd == -1)
            throw system_error("socket",errno);
    }
    unix_socket::~unix_socket() {
        close(sockfd);
    }

    void unix_socket::listen(int backlog){        
        if(::listen(sockfd, 5) == -1)
            throw system_error("listen",errno);
    }

};

#endif//UNIX_SOCKET_H

