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

#include <apache2/httpd.h>
#include <apache2/http_main.h> //server_root
#include <apache2/http_config.h>
#include <apache2/http_request.h>
#include <apache2/http_protocol.h>

#include <apr_file_info.h>      //apr_dir_make_recursive
#include <apr_file_io.h>        //apr_dir_make_recursive

#include <typeinfo>

#include <iostream>

#include "compiler.h"
#include <utils/aprerr.h>
#include <utils/dir.h>

#include "engine.h"
#include "mod_cxxsp.h"
#include "../parser/src/parser.h"

#define CPP_EXT                 ".cpp"
#define SERVLET_EXT             ".sxx"

using namespace std;

template<typename T>
inline cxxsp_conf& get_conf(T *r){
    return *static_cast<cxxsp_conf*>(
        ap_get_module_config(r->server->module_config, &cxxsp_module));
}


inline void make_dir_recursive(const string& dir, apr_pool_t *pool){
    const apr_status_t rv = apr_dir_make_recursive(dir.c_str(),
        APR_UREAD|APR_UWRITE|APR_UEXECUTE, pool);
    if(rv != APR_SUCCESS)
        throw apr_error("apr_dir_make_recursive",rv);
}

void cxxsp_engine::set_config(const cxxsp_conf& conf) {
    compiler.include(conf.servlet_include);
    compiler.include(conf.boost_include);
    compiler.default_flags("-O3 -pipe -funroll-loops");
    //compiler.default_flags("-O3 -pipe -funroll-loops -fomit-frame-pointer");
}

void cxxsp_engine::run_servlet(const string& servlet_path, request_rec *r){
    request_rec* rr = ap_sub_req_lookup_file(servlet_path.c_str(), r, NULL);
    /* Make sure that mod_servlet handles the request (no
        matter what the user config is): */
    rr->handler = "servlet";
    ap_internal_fast_redirect(rr, r);
}

apr_time_t cxxsp_engine::get_mtime(const string& filename, apr_pool_t* pool) {
    apr_finfo_t finfo;
    apr_status_t rv = apr_stat(
        &finfo,
        filename.c_str(),
        APR_FINFO_MTIME,
        pool
    );
    if(rv != APR_SUCCESS)
            return 0;
    return finfo.mtime;
}

int cxxsp_engine::display_exception(request_rec *r, const exception& ex) {
    ap_set_content_type(r, "text/html");
    if(!r->header_only) {
        ap_rputs(DOCTYPE_HTML_4_0T, r);
        ap_rputs("<html><head><title>C++ Server Pages: Exception</title></head>\n",r);
        ap_rputs("<body><h2>C++ Server Pages: Exception</h2>\n",r);
        ap_rprintf(r,"<p>An exception (%s) occured during generation/compilation "
            "of the C++ Server Page.</p>\n",typeid(ex).name());
        ap_rputs("<p>Here is the detailed message:</p>\n",r);
        ap_rputs("<pre style=\"border: solid black 1px; padding: 3px\">",r);
        string msg = ex.what();
        for(string::size_type i = 0;i<msg.size();++i)
            switch(msg[i]) {
                case '<': msg.replace(i,1,"&lt;"); break;
                case '>': msg.replace(i,1,"&gt;"); break;
                case '&': msg.replace(i,1,"&amp;"); break;
                default: break;
            }
        ap_rputs(msg.c_str(),r);
        ap_rputs("</pre>\n",r);
        ap_rputs("</body></html>",r);
    }
    return OK;
}

int cxxsp_engine::process(request_rec *r) {
    try{
        const string source_path = get_conf(r).source_dir +
            strip_ext(r->parsed_uri.path) + CPP_EXT;

        const string servlet_path = get_conf(r).servlet_dir +
            strip_ext(r->parsed_uri.path) + SERVLET_EXT;

        apr_time_t servlet_mtime = get_mtime(servlet_path, r->pool);
        if(r->finfo.mtime > apr_time_now())
            throw runtime_error(string(r->filename)+" has modification set in the future.");
        if(servlet_mtime < r->finfo.mtime) {
            if(r->header_only) {
                return HTTP_PROCESSING;
            }
            make_dir_recursive(directory(source_path), r->pool);
            make_dir_recursive(directory(servlet_path), r->pool);

            cxxsp::parser parser(r->filename, source_path);
            parser.parse();

            string cwd = change_dir_file(r->filename);
            compiler.process(source_path, servlet_path, parser.flags());
            change_dir(cwd);
        }
        run_servlet(servlet_path, r);
        return OK;
    }catch(const not_found_error &ex) {
        display_exception(r,ex);
        return HTTP_NOT_FOUND;
    }catch(const exception& ex){
        return display_exception(r,ex);
    }catch(...) {
        runtime_error ex("Unknown exception");
        return display_exception(r,ex);
    }
}


cxxsp_engine& cxxsp_engine::instance() {
    static cxxsp_engine the_instance;
    return the_instance;
}
