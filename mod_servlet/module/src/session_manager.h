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
#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <HttpSession.h>
#include <HttpServletResponse.h>
#include <httpd.h>
#include <list>

class session_manager{
    std::list<std::string> pending;
    int filedes[2];
public:
    servlet::HttpSession* get(const std::string& id);
    servlet::HttpSession* create(request_rec *r);
    void commit(servlet::HttpServletResponse&);
    std::string generate_session_id(request_rec* r) const;
    static session_manager& instance();
};

#endif//SESSION_MANAGER_H

