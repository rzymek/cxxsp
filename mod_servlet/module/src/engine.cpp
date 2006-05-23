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
/***************************************************************************
 *            servlet_engine.cpp
 *
 *  Thu Sep 30 17:22:19 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "engine.h"
#include "session_manager.h"
#include <stdexcept>
#include <exception>
#include <unknown_exception.h>

#include <HttpServletResponseImpl.h>
#include <HttpServletRequestImpl.h>
#include <servlet/HttpServlet.h>

#include <http_protocol.h>
#include <http_config.h>
#include <httpd.h>
#include <typeinfo>
#include <utils/syserr.h>
#include "loader.h"
#include "ipr.h"
#include <utils/dir.h>

#include <fstream>

using namespace std;

const string IPR_SOCKET_PATH = "/tmp/servlet_socket";

inline int display_exception(const exception& ex, request_rec* r,
        servlet::HttpServletResponse& response)
{
        if (r->header_only)
                return OK;
        ap_set_content_type(r, "text/html");
        ap_rputs(">\n",r);
        ap_rvputs(r,
                "<hr>\n<h2>An exception occured</h2>\n",
                "<dl>\n"
                "<dt>Request URI: <dd>", r->uri, "\n",
                "<dt>Correspoinding file: <dd>", r->filename, "\n",
                "<dt>Message: <dd><b>", ex.what(), "</b>\n",
                "<dt>Exception type: <dd>", typeid(ex).name(),"\n",
                "</dl><hr>\n",
                NULL);
        return OK;
}

extern "C" module AP_MODULE_DECLARE_DATA servlet_module;

int servlet::engine::process(request_rec * r) {
        cout << "servlet::engine::process(" << r->filename << ")" << endl;
        HttpServletRequestImpl request(r);
        HttpServletResponseImpl response(r);
        request.response = &response;
        if(r->prev) { //we're in a redirect/subrequest
            HttpServletRequestImpl* prev = reinterpret_cast<HttpServletRequestImpl*>(
                ap_get_module_config(r->prev->request_config, &servlet_module)
            );
            request.attributes = prev->attributes;
            request.cookies = prev->cookies;
            request.session = prev->session;
            r->parsed_uri = r->prev->parsed_uri;
            r->headers_out = r->prev->headers_out;
        }
        try {
                servlet::loader& loader = servlet::loader::instance();
                servlet::HttpServlet& servlet = loader.get(r);

                string cwd = change_dir_file(r->filename);

                servlet.service(
                    static_cast<servlet::ServletRequest&>(request),
                    static_cast<servlet::ServletResponse&>(response)
                );

                change_dir(cwd);

                response.flushBuffer();
                return response.resultCode;
        }catch(const not_found_error &ex) {
                display_exception(ex,r,response);
                return HTTP_NOT_FOUND;
        }catch(const exception &ex) {
                return display_exception(ex,r,response);
        }catch(char* str){
                runtime_error ex(str);
                return display_exception(ex,r,response);
        }catch(...) {
                static unknown_exception ex;
                return display_exception(ex,r,response);
        }
}

template<typename T>
apr_status_t delete_cleanup(void* obj) {
    delete reinterpret_cast<T*>(obj);
    return OK;
}

void servlet::engine::child_init(server_rec* s) {
    ofstream log("/home/rzymek/devel/00cxxsp/profile/servlet.log", ios_base::app);
    log << "child_init: " << getpid() << endl;
    log.close();

    cout << "PID: " << getpid() << endl;

    ipr::client* ipr_client = new ipr::client(IPR_SOCKET_PATH);
    apr_pool_userdata_set(
        ipr_client,
        "ipr::client",
        &delete_cleanup<ipr::client>,
        s->process->pconf //or s->process->pool (???)
    );
}

void servlet::engine::parent_init(server_rec* s) {
    pid_t pid = ipr::server::dispatch(IPR_SOCKET_PATH);

    ofstream log("/home/rzymek/devel/00cxxsp/profile/servlet.log", ios_base::app);
    log << "parent_init: " << getpid() << endl;
    log << "IPR process is " << pid << endl;
    log.close();

    // We need to wait for initialization of the ipr::server to finish
    //before apache starts initializing children
    sleep(1); //TODO: smth better.
}
