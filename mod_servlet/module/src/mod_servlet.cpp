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
#include <exception>
// #include <stdexcept>

#include <httpd.h>
#include <http_config.h>
#include "loader.h"
#include <HttpServlet.h>
#include <HttpServletResponse.h>
#include <HttpServletRequest.h>
#include "engine.h"
#include "session_manager.h"

using namespace std;

static int servlet_handler (request_rec *r) {
    if (strcmp(r->handler, "servlet"))
        return DECLINED;
//     servlet_engine& engine = servlet_engine::instance();
    return servlet::engine::process(r);
}

static void servlet_child_init(apr_pool_t *p, server_rec *s) {
    servlet::engine::child_init(s);
}

static int servlet_post_config(apr_pool_t *pconf, apr_pool_t *plog,
    apr_pool_t *ptemp, server_rec *s)
{
    void *data = NULL;
    bool first_time = false;
    const char *userdata_key = "servlet_init_module";

    apr_pool_userdata_get(&data, userdata_key, s->process->pool);
    if (data == NULL) {
        // This is entered only on the first time module is loaded.
        first_time = true;
        apr_pool_userdata_set((const void *)1, userdata_key,
            apr_pool_cleanup_null, s->process->pool);
    }

    if (!first_time) {
        /* This section is entered when the module is loaded
           for the second and last time */

        servlet::engine::parent_init(s);
    }

    return OK;
}

static void servlet_register_hooks (apr_pool_t *p) {
    ap_hook_handler(servlet_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_config(servlet_post_config, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(servlet_child_init, NULL, NULL, APR_HOOK_MIDDLE);
}

extern "C" module AP_MODULE_DECLARE_DATA servlet_module;
module AP_MODULE_DECLARE_DATA servlet_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    servlet_register_hooks,
};
