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

#include "engine.h"
#include "mod_cxxsp.h"

#define DEFAULT_SOURCE_DIR      "cxxsp/work"
#define DEFAULT_SERVLET_DIR     "cxxsp/servlets"
#define DEFAULT_SERVLET_INCLUDE "/usr/include"
#define DEFAULT_BOOST_INCLUDE   "/usr/include"

using namespace std;

extern "C" module AP_MODULE_DECLARE_DATA cxxsp_module;

static int cxxsp_handler (request_rec *r) {
    if(strcmp(r->handler, "cxxsp"))
        return DECLINED;
    /* If there was no redirection made (because there was a
    error during generation/compilation) we have written an error message.
    Now we need to say that we have handled the request (and not mod_servlet)
    */
    return OK;
}

template<typename T>
inline cxxsp_conf& get_conf(T *r){
    return *static_cast<cxxsp_conf*>(
        ap_get_module_config(r->server->module_config, &cxxsp_module));
}

static int cxxsp_fixups(request_rec *r) {
    if(!r->handler || strcmp(r->handler, "cxxsp"))
        return DECLINED;
    cxxsp_engine& engine = cxxsp_engine::instance();
    return engine.process(r);
}

static void *create_cxxsp_config(apr_pool_t *p, server_rec *s) {
    cxxsp_conf* conf = reinterpret_cast<cxxsp_conf*>(
        apr_pcalloc(p, sizeof(cxxsp_conf)));
    conf->source_dir = ap_server_root_relative(p, DEFAULT_SOURCE_DIR);
    conf->servlet_dir = ap_server_root_relative(p, DEFAULT_SERVLET_DIR);
    conf->servlet_include = DEFAULT_SERVLET_INCLUDE;
    return conf;
}


static int cxxsp_post_config(apr_pool_t *pconf, apr_pool_t *plog,
    apr_pool_t *ptemp, server_rec *s)
{
    cxxsp_engine& engine = cxxsp_engine::instance();
    cxxsp_conf conf = *static_cast<cxxsp_conf*>(
        ap_get_module_config(s->module_config, &cxxsp_module)
    );
    engine.set_config(conf);
    return OK;
}

static const char *set_source_dir(cmd_parms *cmd, void* member_index, const char *arg) {
    get_conf(cmd).source_dir = arg;
    return NULL;
}
static const char *set_servlet_dir(cmd_parms *cmd, void* member_index, const char *arg) {
    get_conf(cmd).servlet_dir= arg;
    return NULL;
}
static const char *set_servlet_include(cmd_parms *cmd, void* member_index, const char *arg) {
    get_conf(cmd).servlet_include = arg;
    return NULL;
}
static const char *set_boost_include(cmd_parms *cmd, void* member_index, const char *arg) {
    get_conf(cmd).boost_include = arg;
    return NULL;
}

static const command_rec cxxsp_cmds[] =
{
    AP_INIT_TAKE1("C++SourceDir", reinterpret_cast<const char*(*)()>(set_source_dir),
        NULL, RSRC_CONF,
        "directory where temporary c++ source file are put. "
        "Default: " DEFAULT_SOURCE_DIR),
    AP_INIT_TAKE1("C++ServletDir", reinterpret_cast<const char*(*)()>(set_servlet_dir),
        NULL, RSRC_CONF,
        "directory where servlets generated from C++ServerPages are put."
        "Files from this dir must be allowed to be served."
        "Default: " DEFAULT_SERVLET_DIR),
    AP_INIT_TAKE1("ServletInclude", reinterpret_cast<const char*(*)()>(set_servlet_include),
        NULL, RSRC_CONF,
        "path to servlet API header files (from mod_servlet). "
        "Default: " DEFAULT_SERVLET_INCLUDE),
    AP_INIT_TAKE1("BoostInclude", reinterpret_cast<const char*(*)()>(set_boost_include),
        NULL, RSRC_CONF,
        "path to Boost header files. "
        "Default: " DEFAULT_BOOST_INCLUDE),
    { NULL }
};

static void cxxsp_register_hooks (apr_pool_t *p) {
/* ap_internal_fast_redirect doesn't want to work when called from
   a ap_hook_handler registered function. ap_hook_fixups works fine. */
    ap_hook_fixups(cxxsp_fixups, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(cxxsp_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_config(cxxsp_post_config, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA cxxsp_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    create_cxxsp_config,
    NULL,
    cxxsp_cmds,
    cxxsp_register_hooks,
};
