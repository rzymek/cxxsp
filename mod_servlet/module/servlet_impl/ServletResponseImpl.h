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
#ifndef SERVLETRESPONSEIMPL_H
#define SERVLETRESPONSEIMPL_H

#include <string>
#include <servlet/HttpServletResponse.h>
#include <apachestream.h>

struct request_rec;

class ServletResponseImpl: public servlet::HttpServletResponse {
    oapachestream output_stream;
    std::string content_type;
public:
    request_rec *r;

    ServletResponseImpl(request_rec* r);
    virtual ~ServletResponseImpl()
        { }
    std::string getCharacterEncoding();
    std::string getContentType();
    std::ostream& getOutputStream();
    void setCharacterEncoding(const std::string& charset);
    void setContentLength(int len);
    void setContentType(const std::string& type);
    void setBufferSize(int size);
    int  getBufferSize();
    void flushBuffer();
    bool isCommitted();
    void reset();
//    void setLocale(const Locale &locale);
//    const Locale& getLocale();
};

#endif//SERVLETRESPONSEIMPL_H
