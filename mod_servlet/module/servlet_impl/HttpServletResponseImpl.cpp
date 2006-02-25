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
#include <sstream>
#include <stdexcept>

#include <httpd.h>
#include <http_protocol.h>

#include <HttpServletResponseImpl.h>

#include <iostream>//dbg

using namespace std;
using namespace servlet;

//----------------------------------------------------------------------------

HttpServletResponseImpl::HttpServletResponseImpl(request_rec* req)
        : ServletResponseImpl(req), resultCode(OK)
{
}
//----------------------------------------------------------------------------
//HttpServletResponse:
void  HttpServletResponseImpl::addCookie(const Cookie& cookie) {
    stringstream value;
    value << cookie.getName() + "=" + cookie.getValue();
    if(!cookie.getComment().empty())
        value << "; Comment=" << cookie.getComment();
    if(!cookie.getDomain().empty())
        value << "; Domain=" << cookie.getDomain();
    if(cookie.getMaxAge() >= 0)
        value << "; Max-Age=" << cookie.getMaxAge();
    if(!cookie.getPath().empty())
        value << "; Path=" << cookie.getPath();
    if(cookie.getSecure())
        value << "; Secure";
    value << "; Version=" << cookie.getVersion();
    addHeader("Set-Cookie", value.str().c_str());
}
//----------------------------------------------------------------------------
bool  HttpServletResponseImpl::containsHeader(const std::string& name) const {
    throw runtime_error("TODO: HttpServletResponseImpl::containsHeader(const std::string& name)");
}
//----------------------------------------------------------------------------
std::string  HttpServletResponseImpl::encodeURL(const std::string& url) {
    throw runtime_error("TODO: HttpServletResponseImpl::encodeURL(const std::string& url)");
}
//----------------------------------------------------------------------------
std::string  HttpServletResponseImpl::encodeRedirectURL(const std::string& url) {
    throw runtime_error("TODO: HttpServletResponseImpl::encodeRedirectURL(const std::string& url)");
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::sendError(int sc, const std::string& msg) {
    if(!msg.empty())
        getOutputStream() << msg << endl;
    resultCode = sc;
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::sendError(int sc) {
    sendError(sc,"");
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::sendRedirect(const std::string& location) {
    resultCode = HTTP_MOVED_TEMPORARILY; //FOUND
    setHeader("Location",location);
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::setDateHeader(const std::string& name, long date) {
    throw runtime_error("TODO: HttpServletResponseImpl::setDateHeader(const std::string& name, long date)");
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::addDateHeader(const std::string& name, long date) {
    throw runtime_error("TODO: HttpServletResponseImpl::addDateHeader(const std::string& name, long date)");
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::setHeader(const std::string& name, const std::string& value) {
    apr_table_set(r->headers_out, name.c_str(), value.c_str());
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::addHeader(const std::string& name, const std::string& value) {
    apr_table_add(r->headers_out, name.c_str(), value.c_str());
}
//----------------------------------------------------------------------------
void  HttpServletResponseImpl::setIntHeader(const std::string& name, int value) {
    throw runtime_error("TODO: HttpServletResponseImpl::setIntHeader(const std::string& name, int value)");
}
//----------------------------------------------------------------------------
void HttpServletResponseImpl::addIntHeader(const std::string& name, int value) {
    throw runtime_error("TODO: HttpServletResponseImpl::addIntHeader(const std::string& name, int value)");
}
//----------------------------------------------------------------------------
void HttpServletResponseImpl::setStatus(int sc) {
    throw runtime_error("TODO: HttpServletResponseImpl::setStatus(int sc)");
}
//----------------------------------------------------------------------------
