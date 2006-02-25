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

#ifndef CXXSP_ENGINE_H
#define CXXSP_ENGINE_H

#include <string>
#include <stdexcept>
#include <apache2/httpd.h>

#include "compiler.h"
#include "mod_cxxsp.h"

class cxxsp_engine {
private:
    cxxsp_engine()
        { }
    cxxsp_engine(const cxxsp_engine&);

    int display_exception(request_rec *r, const std::exception& ex);
    apr_time_t get_mtime(const std::string& filename, apr_pool_t* pool);
    void run_servlet(const std::string& servlet_path, request_rec *r);

    cxxsp::compiler compiler;
    cxxsp_conf conf;
public:
    void set_config(const cxxsp_conf& conf);

    int process(request_rec *r);

    static cxxsp_engine& instance();

    friend class xxx;//suppress warn. about private constructors
};

#endif//CXXSP_ENGINE_H

