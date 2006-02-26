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
#ifndef HTTPSERVLETREQUESTIMPL_H
#define HTTPSERVLETREQUESTIMPL_H

#include <string>
#include <vector>
#include <map>
#include <servlet/HttpServletRequest.h>
#include <servlet/Cookie.h>
#include <servlet/HttpSession.h>
#include "HttpSessionImpl.h"
#include "ServletRequestImpl.h"

struct request_rec;
namespace servlet {
    class HttpServletResponse;
}

class HttpServletRequestImpl: public ServletRequestImpl {

    mutable std::vector<servlet::Cookie> cookies;
    mutable bool cookies_parsed;
    const std::vector<servlet::Cookie>& parse_cookies() const;

    mutable std::multimap<std::string,std::string> headers;
    const std::multimap<std::string,std::string>& parse_headers() const;

    mutable std::multimap<std::string,std::string> parameters;
    const std::multimap<std::string,std::string>& parse_parameters() const;

    const std::multimap<std::string,std::string>& getHeadersMap() const
        { return headers.empty() ? parse_headers() : headers; }

    std::string generate_session_id() const;

    mutable std::auto_ptr<HttpSessionImpl> session;
    inline const std::vector<servlet::Cookie>& __getCookies() const {
        return (cookies_parsed ? cookies : parse_cookies());
    }
public:
    servlet::HttpServletResponse* response;

    HttpServletRequestImpl(request_rec* r);
    //HttpServletRequest:
    std::string  getAuthType() const;
    std::auto_ptr< std::vector<servlet::Cookie> > getCookies() const ;
    long  getDateHeader(const std::string& name) const;
    std::string  getHeader(const std::string& name) const ;
    std::auto_ptr< std::vector<std::string> > getHeaders(const std::string& name) const ;
    std::auto_ptr< std::vector<std::string> >  getHeaderNames() const ;
    int  getIntHeader(const std::string& name) const ;
    std::string  getMethod() const ;
    std::string  getPathInfo() const ;
    std::string  getPathTranslated() const ;
    std::string  getContextPath() const ;
    std::string  getQueryString() const ;
    std::string  getRemoteUser() const ;
    bool isUserInRole(const std::string& role) const ;
//  const Principal& getUserPrincipal();
    std::string  getRequestedSessionId() const ;
    std::string  getRequestURI() const ;
    std::string  getRequestURL() const ;
    std::string  getServletPath() const ;
    servlet::HttpSession* getSession(bool create = true) const ;
    bool  isRequestedSessionIdValid() const ;
    bool  isRequestedSessionIdFromCookie() const ;
    bool  isRequestedSessionIdFromURL() const ;

    friend int cookie_iterator(void* rec, const char *key, const char *value);
    friend int servlet::engine::process(request_rec*);
};

#endif//HTTPSERVLETREQUESTIMPL_H
