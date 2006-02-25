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
#include <http_core.h> //apr_get_remote_host
#include <http_config.h>
#include <util_filter.h>

#include "ServletRequestImpl.h"
//#include <Locale.h>
#include <RequestDispatcherImpl.h>

#include <session_manager.h>
#include <utils/map.h>
#include <utils/mem.h>
#include <utils/http.h>
#include <utils/aprerr.h>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace boost;
using namespace servlet;

#define nstr(s) ( s ? s : string() )

//----------------------------------------------------------------------------
ServletRequestImpl::ServletRequestImpl(request_rec* req)
        : r(req)
{
}
//----------------------------------------------------------------------------
bool ServletRequestImpl::hasAttribute(const string& name) const {
    return (attributes.find(name) != attributes.end());
}
//----------------------------------------------------------------------------
const boost::any& ServletRequestImpl::__getAttributeImpl(const string& name) const {
    return attributes[name];
    //~ attributes_t::const_iterator entry = attributes.find(name);
    //~ if(entry == attributes.end())
        //~ throw runtime_error("No such attribute: " + name);
    //~ return entry->second;
}
//----------------------------------------------------------------------------
auto_ptr< vector<string> >  ServletRequestImpl::getAttributeNames() const{
    return get_keys(attributes);
}
//----------------------------------------------------------------------------
void  ServletRequestImpl::__setAttributeImpl(const string& name, const boost::any& value) {
    attributes[name] = value;
}
//----------------------------------------------------------------------------
void  ServletRequestImpl::removeAttribute(const string& name){
    attributes.erase(name);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getCharacterEncoding() const{
    throw runtime_error("TODO ServletRequestImpl::getCharacterEncoding()");
}
//----------------------------------------------------------------------------
void  ServletRequestImpl::setCharacterEncoding(const string& env){
    throw runtime_error("TODO ServletRequestImpl::setCharacterEncoding(const string& env)");
}
//----------------------------------------------------------------------------
int  ServletRequestImpl::getContentLength() const{
    return static_cast<int>(r->clength);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getContentType() const{
    return nstr(r->content_type);
}
//----------------------------------------------------------------------------
istream&  ServletRequestImpl::getInputStream() const{
    throw runtime_error("TODO ServletRequestImpl::getInputStream()");
}
//----------------------------------------------------------------------------
string ServletRequestImpl::getParameter(const string& name) const{
    return get_value(*getParameterMap(),name);
}
//----------------------------------------------------------------------------
auto_ptr< vector<string> >  ServletRequestImpl::getParameterNames() const{
    return get_keys(*getParameterMap());
}
//----------------------------------------------------------------------------
auto_ptr< vector<string> > ServletRequestImpl::getParameterValues(const string& name) const{
    return get_values(*getParameterMap(), name);
}
string ServletRequestImpl::read_POST() const{
    //Author: Nick Kew <nick@webthing.com>, GPL
    //TODO: move this to iapachestream
    const apr_off_t BLOCKSIZE = 256;

    string result;
    const char* buf ;
    apr_size_t bytes ;

    shared_ptr<apr_bucket_brigade> bb(
        apr_brigade_create(r->pool, r->connection->bucket_alloc),
        apr_brigade_destroy
    );
    for(;;) {
        apr_status_t status =
            ap_get_brigade(r->input_filters, bb.get(), AP_MODE_READBYTES, APR_BLOCK_READ, BLOCKSIZE) ;
        if ( status != APR_SUCCESS )
            throw apr_error("ap_get_brigade", status);
        for (apr_bucket* b = APR_BRIGADE_FIRST(bb); b != APR_BRIGADE_SENTINEL(bb); b = APR_BUCKET_NEXT(b) ) {
            if ( APR_BUCKET_IS_EOS(b) ) {
                return result;
            } else {
                status = apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
                if(status != APR_SUCCESS)
                    throw apr_error("apr_bucket_read", status);

                result.append(buf,bytes);
            }
        }
        apr_brigade_cleanup(bb.get()) ;
    }

}
//----------------------------------------------------------------------------
void ServletRequestImpl::split_to_parameters(const string& query) const {
    const static char_separator<char> sep("&");
    typedef tokenizer< char_separator<char> > tokenizer;
    tokenizer tokens(query, sep);
    vector<string> nv(2);
    for(tokenizer::iterator token = tokens.begin(); token != tokens.end(); ++token) {
        nv.clear();
        split(nv, *token, is_any_of("="));
        nv.resize(2);
        replace_all(nv[0], "+", " ");
        replace_all(nv[1], "+", " ");
        nv[0] = unescape(nv[0]);
        nv[1] = unescape(nv[1]);
        parameters.insert(make_pair(nv[0], nv[1] ));
    }
}
//----------------------------------------------------------------------------
const multimap<string,string>& ServletRequestImpl::parse_parameters() const{
    parameters.clear();
    string query = nstr(r->parsed_uri.query);
    split_to_parameters(query);
    if ( r->method_number == M_POST )
        split_to_parameters(read_POST());
    return parameters;
}
auto_ptr< multimap<string,string> >  ServletRequestImpl::getParameterMap() const{
    if(parameters.empty() && (r->parsed_uri.query || r->method_number == M_POST))
         parse_parameters();
    return make_auto_ptr(parameters);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getProtocol() const {
    return nstr(r->protocol);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getScheme() const{
    throw runtime_error("TODO ServletRequestImpl::getScheme()");
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getServerName() const{
    return nstr(r->server->server_hostname);
}
//----------------------------------------------------------------------------
int  ServletRequestImpl::getServerPort() const{
    return static_cast<int>(r->server->port);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getRemoteAddr() const{
    return r->connection->remote_ip;
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getRemoteHost() const{
    return ap_get_remote_host(r->connection, r->per_dir_config, REMOTE_NAME, NULL);
}
//----------------------------------------------------------------------------
/*
Locale ServletRequestImpl::getLocale() const{
    throw runtime_error("TODO ServletRequestImpl::getLocale()");
}
//----------------------------------------------------------------------------
vector<Locale>  ServletRequestImpl::getLocales() const{
    throw runtime_error("TODO ServletRequestImpl::getLocales()");
}
*/
//----------------------------------------------------------------------------
bool  ServletRequestImpl::isSecure() const{
    return boost::starts_with(getProtocol(),"HTTPS");
}
//----------------------------------------------------------------------------
auto_ptr<RequestDispatcher> ServletRequestImpl::getRequestDispatcher(const string& path) const{
    return auto_ptr<RequestDispatcher>(new RequestDispatcherImpl(path));
}
//----------------------------------------------------------------------------
int  ServletRequestImpl::getRemotePort() const{
    apr_port_t port;
    apr_sockaddr_port_get(&port, r->connection->remote_addr);
    return static_cast<int>(port);
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getLocalName() const{
    throw runtime_error("TODO ServletRequestImpl::getLocalName()");
}
//----------------------------------------------------------------------------
string  ServletRequestImpl::getLocalAddr() const{
    throw runtime_error("TODO ServletRequestImpl::getLocalAddr()");
}
//----------------------------------------------------------------------------
int  ServletRequestImpl::getLocalPort() const{
    throw runtime_error("TODO ServletRequestImpl::getLocalPort()");
}
//----------------------------------------------------------------------------
