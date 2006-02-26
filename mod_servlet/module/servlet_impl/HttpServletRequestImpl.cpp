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
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

#include <stdlib.h>//atoi
#include <assert.h>

#include <httpd.h>
#include <apr_date.h> //apr_date_parse_http
#include <apr_general.h>//apr_generate_random_bytes
#include <apr_strings.h> //apr_atoi
#include <apr_pools.h> //apr_pool_userdata_get
#include <http_core.h> //apr_get_remote_host
#include <http_config.h>

#include <servlet/HttpServletRequest.h>
#include "HttpServletRequestImpl.h"
#include <servlet/HttpServletRequest.h>
#include <servlet/Cookie.h>

#include "HttpSessionImpl.h"
//#include <Locale.h>
#include "RequestDispatcherImpl.h"

#include <session_manager.h>
#include <utils/map.h>
#include <utils/mem.h>
#include "../src/ipr.h"

using namespace std;
using namespace servlet;

#define nstr(s) ( s ? s : string() )
#define SESSIONID "CXX_SESSIONID"
#define sessionid "cxx_sessionid"

extern "C" module AP_MODULE_DECLARE_DATA servlet_module;

//----------------------------------------------------------------------------
HttpServletRequestImpl::HttpServletRequestImpl(request_rec* req)
        : ServletRequestImpl(req),cookies_parsed(false),session(NULL)
{
}
//----------------------------------------------------------------------------
//HttpServletRequest:
string  HttpServletRequestImpl::getAuthType() const{
//TODO: check if this is one from BASIC_AUTH, FORM_AUTH, CLIENT_CERT_AUTH, DIGEST_AUTH
    return nstr(r->ap_auth_type);
}
//----------------------------------------------------------------------------
const vector<Cookie>& HttpServletRequestImpl::parse_cookies() const{
    multimap<string,string> all = getHeadersMap();
    vector<Cookie> res;
    for(multimap<string,string>::const_iterator val = all.find("Cookie");
        val != all.end() && val->first == "Cookie"; ++val)
    {
        string name, value;
        int version = 1;
        Cookie *cur = NULL;

        stringstream data(val->second);
        while(data){
            //read the name and value
            data >> ws;
            name.clear();
            value.clear();
            getline(data,name,'=');
            getline(data,value,';');

            if(name.empty())
                if(data)
                    continue; //just empty name, there is still some data
                else
                    break;  //end of input
            if(name[0]=='$') { //special parameters
                if(cur==NULL){
                    //Cookie system parameters
                    if(name=="$Version")
                        version = atoi(value.c_str());
                }else{
                    //Specific cookie parameters
                    if(name=="$Path")
                        cur->setPath(value);
                    else if(name=="$Domain")
                        cur->setDomain(value);
                }
            }else{ //normal cookie data
                Cookie cookie(name, value);
                cookie.setVersion(version);
                version = 1;
                cookies.push_back(cookie);
                cur = &cookies.back();
            }
        }
    }
    cookies_parsed = true;
    return cookies;
}

auto_ptr< vector<Cookie> > HttpServletRequestImpl::getCookies() const {
    return make_auto_ptr(__getCookies());
}
//----------------------------------------------------------------------------
long  HttpServletRequestImpl::getDateHeader(const string& name) const{
    const char* val = apr_table_get(r->headers_in, name.c_str());
    return val ? static_cast<long>(apr_date_parse_http(val)) : -1;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getHeader(const string& name) const {
    return nstr(apr_table_get(r->headers_in, name.c_str()));
}
//----------------------------------------------------------------------------
auto_ptr< vector<string> > HttpServletRequestImpl::getHeaders(const string& name) const {
    return get_values(getHeadersMap(), name);
}
//----------------------------------------------------------------------------
const multimap<string,string>& HttpServletRequestImpl::parse_headers() const{
    assert(headers.empty());
    const apr_array_header_t *elts =
        elts = apr_table_elts(r->headers_in);
    if (elts->nelts == 0)
        return headers;//empty

    const apr_table_entry_t *t_elt =
        (const apr_table_entry_t *)(elts->elts);
    const apr_table_entry_t *t_end =
        t_elt + elts->nelts;
    do {
        headers.insert(
            pair<string,string>(
                nstr(t_elt->key),
                nstr(t_elt->val)
            )
        );
        t_elt++;
    } while (t_elt < t_end);
    return headers;
}
//----------------------------------------------------------------------------
auto_ptr< vector<string> > HttpServletRequestImpl::getHeaderNames() const {
    return get_keys(getHeadersMap());
}
//----------------------------------------------------------------------------
int  HttpServletRequestImpl::getIntHeader(const string& name) const {
    //NOTE: does nothing when value no convertable to int (spec: throw ex)
    const char* value = apr_table_get(r->headers_in, name.c_str());
    return value ? atoi(value) : -1;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getMethod() const {
    if(r->header_only && r->method_number == M_GET)
        return "HEAD";
    return r->method;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getPathInfo() const {
    return nstr(r->path_info);
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getPathTranslated() const {
    return r->path_info ?
        string(r->filename)+r->path_info
        : string();
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getContextPath() const {
    string context_path = nstr(r->parsed_uri.path);
    return (context_path == "/") ? string() : context_path;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getQueryString() const {
    return nstr(r->parsed_uri.query);
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getRemoteUser() const {
    return nstr(r->user);
}
//----------------------------------------------------------------------------
bool HttpServletRequestImpl::isUserInRole(const string& role) const {
    throw runtime_error("TODO HttpServletRequestImpl::isUserInRole(const string& role)");
}
//----------------------------------------------------------------------------
//  const Principal& getUserPrincipal();
string  HttpServletRequestImpl::getRequestedSessionId() const {
    const vector<Cookie>& cookies = __getCookies();
    string id;
    for(unsigned i=0; i<cookies.size(); ++i)
        if(cookies[i].getName() == SESSIONID) {
            id = cookies[i].getValue();
            break;
        }
    if(id.empty())
        id = getParameter(sessionid);
    return id;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getRequestURI() const {
    return r->uri;
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getRequestURL() const {
    stringstream port;
    if(getServerPort() != 80)
        port << getServerPort();
    string query = getQueryString();
    return getProtocol() + "://" + getServerName() + port.str() + getRequestURI() +
        (query.empty() ? ("?"+query) : "");
}
//----------------------------------------------------------------------------
string  HttpServletRequestImpl::getServletPath() const {
    return r->filename;
}
//----------------------------------------------------------------------------
//TODO: cleanup
HttpSession* HttpServletRequestImpl::getSession(bool create) const {
    if(!session.get()) {
        string id = getRequestedSessionId();
        if(id.empty()) {
            id = generate_session_id();
            response->addCookie(Cookie(SESSIONID,id));
        }
        ipr::client* ipr_client;
        apr_pool_userdata_get(
            reinterpret_cast<void**>(&ipr_client),
            "ipr::client",
            r->server->process->pconf
        );
        ipr_client->open("sessions");
        string serialized = ipr_client->get(id);
        session = auto_ptr<HttpSessionImpl>(new HttpSessionImpl(id));
        if(serialized.empty()) {
            session->is_new = false;
            stringstream obj_stream;
            {
                boost::archive::binary_oarchive oa(obj_stream, boost::archive::no_header);
                const HttpSessionImpl& ref = *session;
                oa << ref;
            }
            ipr_client->set(id, obj_stream.str());
            session->is_new = true;
        } else {
            stringstream obj_stream(serialized);
            boost::archive::binary_iarchive ia(
                obj_stream,
                boost::archive::no_header
            );
            HttpSessionImpl& ref = *session;
            ia >> ref;
        }
        session->ipr_client = ipr_client;
    }
    return session.get();
}
//----------------------------------------------------------------------------
bool HttpServletRequestImpl::isRequestedSessionIdValid() const {
    string id = getRequestedSessionId();
    ipr::client* ipr_client;
    apr_pool_userdata_get(
        reinterpret_cast<void**>(&ipr_client),
        "ipr::client",
        r->server->process->pconf
    );
    ipr_client->open("sessions");
    return ipr_client->has(id);
}
//----------------------------------------------------------------------------
bool HttpServletRequestImpl::isRequestedSessionIdFromCookie() const {
    const vector<Cookie>& cookies = __getCookies();
    for(unsigned i=0; i<cookies.size(); ++i)
        if(cookies[i].getName() == SESSIONID)
            return true;
    return false;
}
//----------------------------------------------------------------------------
bool HttpServletRequestImpl::isRequestedSessionIdFromURL() const {
    return !isRequestedSessionIdFromCookie() && !getParameter(sessionid).empty();
}
//----------------------------------------------------------------------------

static string mangle_id(const string& id){
    static const char CHARS[62] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
        'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };
    string mangled;
    mangled.reserve(id.size());
    for(unsigned i=0; i < id.size(); ++i)
        mangled += CHARS[ abs(static_cast<int>(id[i]%sizeof(CHARS))) ];
    return mangled;
}
//----------------------------------------------------------------------------
string HttpServletRequestImpl::generate_session_id() const{
    //Is mod_unique_id is present then USE IT.
    const char* UNIQUE_ID = apr_table_get(r->subprocess_env, "UNIQUE_ID");
    if(UNIQUE_ID)
        return UNIQUE_ID;

    //This is my home-grown method. It sometimes duplicates ids.
    //This came out during JMeter testing with ~50 threads on one computer.
    string id;

    //full request time (including msec)
    char buf[sizeof(r->request_time)+1];
    memcpy(buf, &r->request_time, sizeof(r->request_time));
    buf[sizeof(r->request_time)] = '\0';
    id=buf;

    //port
    apr_port_t port;
    apr_sockaddr_port_get(&port, r->connection->remote_addr);
    memcpy(buf, &port, sizeof(apr_port_t));
    buf[sizeof(int)] = '\0';
    id+=buf;

    //ip
    string::size_type start=0,end;
    string ip = r->connection->remote_ip;
    for(unsigned i=0;i<4;++i){
        end = ip.find('.', start);
        id += (char)atoi(ip.substr(start,end-start).c_str());
        start = end+1;
    }

    //random
    const int size = 8;
    unsigned char randbytes[size];
    apr_generate_random_bytes(randbytes, size);
    id.append(reinterpret_cast<char*>(randbytes), size);

    return mangle_id(id);
}

