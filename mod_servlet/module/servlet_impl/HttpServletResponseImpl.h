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
#ifndef HTTPSERVLETRESPONSEIMPL_H
#define HTTPSERVLETRESPONSEIMPL_H

#include <string>
#include "ServletResponseImpl.h"

struct request_rec;

class HttpServletResponseImpl: public ServletResponseImpl {
public:
    int resultCode;

    HttpServletResponseImpl(request_rec* r);
    virtual ~HttpServletResponseImpl()
        { }
    void addCookie(const servlet::Cookie& cookie);
    bool containsHeader(const std::string& name) const;
    std::string encodeURL(const std::string& url);
    std::string encodeRedirectURL(const std::string& url);
    void sendError(int sc, const std::string& msg);
    void sendError(int sc);
    void sendRedirect(const std::string& location);
    void setDateHeader(const std::string& name, long date);
    void addDateHeader(const std::string& name, long date);
    void setHeader(const std::string& name, const std::string& value);
    void addHeader(const std::string& name, const std::string& value);
    void setIntHeader(const std::string& name, int value);
    void addIntHeader(const std::string& name, int value);
    void setStatus(int sc);
};

#endif//HTTPSERVLETRESPONSEIMPL_H
