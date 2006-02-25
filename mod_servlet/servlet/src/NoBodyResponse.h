/*
 * ====================================================================
 *
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "The Jakarta Project", "Tomcat", and "Apache Software
 *    Foundation" must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 */

#ifndef NO_BODY_RESPONSE_H
#define NO_BODY_RESPONSE_H

#include <string>
#include <iostream>
#include <onobodystream.h>
#include <GenericServlet.h>
#include <ServletRequest.h>
#include <ServletResponse.h>
#include <HttpServletResponse.h>
//#include <Locale.h>

using namespace servlet;

/*
 * A response that includes no body, for use in (dumb) "HEAD" support.
 * This just swallows that body, counting the bytes in order to set
 * the content length appropriately.  All other methods delegate directly
 * to the HTTP Servlet Response object used to construct this one.
 */
class NoBodyResponse : public HttpServletResponse {
    HttpServletResponse &resp;
    onobodystream no_body;
    bool didSetContentLength;

    NoBodyResponse(HttpServletResponse& r);
    virtual ~NoBodyResponse() { }

    void  setContentLength();

        // SERVLET RESPONSE interface methods
public:
    virtual void  setContentLength(int len);
    virtual void  setCharacterEncoding(const std::string& charset);
    virtual void  setContentType(const std::string& type);
    virtual std::string  getContentType();
    virtual std::ostream&  getOutputStream();
    virtual std::string  getCharacterEncoding();
    virtual void  setBufferSize(int size);
    virtual int  getBufferSize();
    virtual void  reset();
    virtual bool  isCommitted();
    virtual void  flushBuffer();
    //virtual void  setLocale(const Locale& loc);
    //virtual const Locale&  getLocale();
        // HTTP SERVLET RESPONSE interface methods
    virtual void  addCookie(const Cookie& cookie);
    virtual bool  containsHeader(const std::string& name) const;
    virtual void  setStatus(int sc);
    virtual void  setHeader(const std::string& name, const std::string& value);
    virtual void  setIntHeader(const std::string& name, int value);
    virtual void  setDateHeader(const std::string& name, long date);
    virtual void  sendError(int sc, const std::string& msg);
    virtual void  sendError(int sc);
    virtual void  sendRedirect(const std::string& location);
    virtual std::string encodeURL(const std::string& url);
    virtual std::string  encodeRedirectURL(const std::string& url);
    virtual void  addHeader(const std::string& name, const std::string& value);
    virtual void  addDateHeader(const std::string& name, long value);
    virtual void  addIntHeader(const std::string& name, int value);

        friend class HttpServlet;
};
#endif/*NO_BODY_RESPONSE_H*/
