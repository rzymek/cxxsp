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
#include <servlet/HttpServlet.h>

#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include "apr_time.h"

#include "../module/src/loader.h"

using namespace std;
using namespace servlet;

class Manager : public HttpServlet {
public:

    void doGet(HttpServletRequest& req, HttpServletResponse &res) {
        res.setContentType("text/plain");
        ostream& out = res.getOutputStream();
        out << "pid: " << getpid() << endl << endl;

        string action = req.getParameter("action");
        if(action == "list") {
            servlet::loader& loader = servlet::loader::instance();
            out << "Loaded servlets(" << loader.pool.size() << "):" << endl;
            int cnt = 0;
            char timebuf[256];
            for(std::map<std::string, servlet::loader::pool_entry>::const_iterator i = loader.pool.begin();
                i != loader.pool.end(); ++i)
            {
                apr_rfc822_date(timebuf, i->second.atime);
                HttpServlet* servlet = i->second.servlet;
                out << ++cnt << ": " << (servlet ? typeid(*servlet).name() : "NULL") <<
                    "\t(Access: " << timebuf << ", Load: ";
                apr_rfc822_date(timebuf, i->second.ltime);
                out << timebuf << ")" << endl;
            }
        }else if(action == "load") {
            string path = req.getParameter("path");
            servlet::loader::instance().load(path);
            out << "Loaded " << path << endl;
        }else {
            out << "Unknown action: " << action << endl;
        }
    }
};

extern "C" HttpServlet* create() { return new Manager(); }
