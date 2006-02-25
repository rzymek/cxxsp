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
#include <RequestDispatcherImpl.h>
#include <HttpServletRequestImpl.h>
#include <apache2/http_request.h>
#include <apache2/http_config.h>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
using namespace std;
using namespace boost;
using namespace servlet;

extern "C" module AP_MODULE_DECLARE_DATA servlet_module;

inline string get_request_dir(request_rec* r) {
    string dir = r->parsed_uri.path;
    if(r->path_info)
        erase_tail(dir, strlen(r->path_info));
    dir.erase(dir.find_last_of('/')+1);
    return dir;
}

void RequestDispatcherImpl::forward(ServletRequest& request, ServletResponse& response) const {
    HttpServletRequestImpl& req = static_cast<HttpServletRequestImpl&>(request);
    ap_set_module_config(req.r->request_config, &servlet_module,
        const_cast<void*>(reinterpret_cast<const void*>(&req))
    );
    const string url = starts_with(path,"/") ? path : get_request_dir(req.r) + path;
    ap_internal_redirect(url.c_str(), req.r);
}

void RequestDispatcherImpl::include(ServletRequest& request, ServletResponse& response) const {
    HttpServletRequestImpl& req = static_cast<HttpServletRequestImpl&>(request);
    const string url = starts_with(path,"/") ? path : get_request_dir(req.r) + path;
    request_rec* rr = ap_sub_req_lookup_uri(url.c_str(), req.r, NULL);
    ap_run_sub_req(rr);
    ap_destroy_sub_req(rr);
}
