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
#include <dlfcn.h>                              //dlopen,dlsym,dlerror,dlclose
#include "loader.h"
#include <utils/syserr.h>
#include <httpd.h>

#include <sstream>
#include <stdexcept>

#include <iostream>//dbg

using namespace std;

namespace servlet {
    struct has_second_equal_to  {
        has_second_equal_to(loader::pool_entry& e)
            : entry(e) { }
        loader::pool_entry& entry;
        bool operator()(const pair<string,loader::pool_entry>& x)
            { return &entry == &x.second; }
    };
    void loader::unload(pool_entry & entry) {
//         cout << "loader::unload" << endl;
        destroy_servlet(entry.servlet);
        delete entry.handle;
        entry.handle = NULL;

        pool.erase(find_if(
            pool.begin(),
            pool.end(),
            has_second_equal_to(entry)
        ));
    }
/*
    void loader::load(pool_entry & entry, request_rec* r) {
        cout << "loader::load" << endl;
        auto_ptr<dso_code> code( new dso_code(r->filename, r->finfo.mtime) );
        entry.servlet = create_servlet(code.get());
        entry.handle = code.release();
    }
*/
    HttpServlet* loader::create_servlet(dso_code* code) {
//         cout << "loader::create_servlet" << endl;
        auto_ptr<HttpServlet> servlet( code->call<HttpServlet*>("create") );
        if (!servlet.get())
                throw runtime_error("Error loading servlet module. Servlet creation function returned NULL. ");
        servlet->init();
        return servlet.release();
    }

    void loader::destroy_servlet(HttpServlet* servlet) {
//         cout << "loader::destroy_servlet" << endl;
        servlet->destroy();
        delete servlet;
    }

    HttpServlet& loader::get(request_rec* r) {
//         cout << "loader::get" << endl;
        if (r->finfo.filetype == APR_NOFILE)
                throw not_found_error(r->filename);
        if (r->finfo.filetype != APR_REG)
                throw runtime_error("Requested file is not a regural file. ");
        pool_entry &entry = load(r->filename);
        entry.atime = r->request_time;
        return *entry.servlet;
    }

    loader::pool_entry& loader::load(const string& path) {
        pool_entry &entry = pool[path];
        if(entry.handle) {
            if(entry.handle->file_changed()) {
                destroy_servlet(entry.servlet);
                entry.handle->reload_if_needed();
                entry.servlet = create_servlet(entry.handle);
                entry.ltime = apr_time_now();
            }
        } else{
            auto_ptr<dso_code> code( new dso_code(path) );
            entry.servlet = create_servlet(code.get());
            entry.handle = code.release();
            entry.ltime = apr_time_now();
        }
        return entry;
    }

    loader& loader::instance(){
        static loader instance;
        return instance;
    }
}
