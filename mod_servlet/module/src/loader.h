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
#ifndef LOADER_H
#define LOADER_H

#include <map>
#include <string>
#include <httpd.h>
#include <apr_file_info.h>
#include "dso_code.h"
#include <apr_pools.h>
#include <HttpServlet.h>


#include <iostream>

namespace servlet {

    class loader {
    public:
            struct pool_entry {
                    dso_code *handle;
                    HttpServlet *servlet;
                    apr_time_t atime,//last access time
                               ltime;
            };
    private:
            loader() { /* std::cout << "\t\t\tNEW LOADER"<<std::endl;*/ }
            loader(const loader&);//forbid
            HttpServlet* create_servlet(dso_code* code);
            void destroy_servlet(HttpServlet*);
    protected:
            void unload(pool_entry& entry);//, request_rec* r);
//             void load(pool_entry& entry, request_rec* r);
    public:
            std::map<std::string, pool_entry> pool;
            pool_entry& load(const std::string& path);

            HttpServlet& get(request_rec* request);
            static loader& instance();
    };
}
#endif
