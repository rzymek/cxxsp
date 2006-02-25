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
#include "unix_socket.h"

namespace net {

    #ifndef UNIX_PATH_MAX
        #define UNIX_PATH_MAX 108
    #endif
    void unix_socket::bind(const std::string& sun_path) {
        struct sockaddr_un local;
        local.sun_family = AF_UNIX;
        strncpy(local.sun_path,sun_path.c_str(), UNIX_PATH_MAX);
        unlink(local.sun_path); 
        if(::bind(sockfd, reinterpret_cast<sockaddr*>(&local), sizeof(local)) == -1)
            throw system_error("bind",errno);
    }

    int unix_socket::accept() {
        int client_socket = ::accept(sockfd, NULL, NULL);
        if(client_socket == -1)
            throw system_error("accept",errno);
        return client_socket;
    }

    int unix_socket::connect(const std::string& sun_path) {
        struct sockaddr_un local;
        local.sun_family = AF_UNIX;
        strncpy(local.sun_path,sun_path.c_str(), UNIX_PATH_MAX);
        
        if(::connect(sockfd, reinterpret_cast<sockaddr*>(&local), sizeof(local)) == -1)
            throw system_error("connect",errno);
        return sockfd;        
    }
}


