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
#ifndef SERVLETREQUESTIMPL_H
#define SERVLETREQUESTIMPL_H

#include <string>
#include <vector>
#include <map>
#include <boost/any.hpp>
#include <servlet/HttpServletRequest.h>


struct request_rec;
namespace servlet {
    namespace engine {
        extern int process(request_rec*);
    };
};

class ServletRequestImpl: public servlet::HttpServletRequest {
    mutable std::multimap<std::string,std::string> headers;
    const std::multimap<std::string,std::string>& parse_headers() const;

    typedef std::multimap<std::string,std::string> parameters_t;
    mutable parameters_t parameters;
    const parameters_t& parse_parameters() const;
    std::string read_POST() const;
    void split_to_parameters(const std::string& query) const;

    typedef std::map<std::string, boost::any> attributes_t;
    mutable attributes_t attributes;
public:
    request_rec *r;

    ServletRequestImpl(request_rec* r);
    virtual ~ServletRequestImpl()
        {   }

    //ServletRequest:
    const boost::any& __getAttributeImpl(const std::string& name) const;
    bool  hasAttribute(const std::string& name) const;
    std::auto_ptr< std::vector<std::string> > getAttributeNames() const;
    void  __setAttributeImpl(const std::string& name, const boost::any& value);
    void  removeAttribute(const std::string& name);

    std::string  getCharacterEncoding() const;
    void  setCharacterEncoding(const std::string& env);
    int  getContentLength() const;
    std::string  getContentType() const;
    std::istream&  getInputStream() const;
    std::string  getParameter(const std::string& name) const;
    std::auto_ptr< std::vector<std::string> > getParameterNames() const;
    std::auto_ptr< std::vector<std::string> > getParameterValues(const std::string& name) const;
    std::auto_ptr<parameters_t>  getParameterMap() const;
    std::string  getProtocol() const;
    std::string  getScheme() const;
    std::string  getServerName() const;
    int  getServerPort() const;
    std::string  getRemoteAddr() const;
    std::string  getRemoteHost() const;
    //Locale getLocale() const;
    //std::vector<Locale>  getLocales() const;
    bool  isSecure() const;
    std::auto_ptr<servlet::RequestDispatcher> getRequestDispatcher(const std::string& path) const;
    int  getRemotePort() const;
    std::string  getLocalName() const;
    std::string  getLocalAddr() const;
    int  getLocalPort() const;

    friend int servlet::engine::process(request_rec*);
};

#endif//SERVLETREQUESTIMPL_H
