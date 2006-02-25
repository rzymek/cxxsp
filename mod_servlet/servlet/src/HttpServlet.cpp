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

#include <stdexcept>
#include <HttpServlet.h>
#include <NoBodyResponse.h>
#include <HttpServletRequest.h>
#include <HttpServletResponse.h>
#include <ServletRequest.h>
#include <ServletResponse.h>
#include <boost/algorithm/string/predicate.hpp>
#include <httpd.h>

using namespace std;
using namespace servlet;


inline void not_supported(HttpServletRequest& req, HttpServletResponse& resp,
    const string& method_name)
{
    const string protocol = req.getProtocol();
    const string msg = "Method "+method_name+" not supported";
    if (boost::ends_with(protocol, "1.1"))
        resp.sendError(HttpServletResponse::SC_METHOD_NOT_ALLOWED, msg);
    else
        resp.sendError(HttpServletResponse::SC_BAD_REQUEST, msg);
}
//--------------------------------------------------------------------

void  HttpServlet::doGet(HttpServletRequest& req,  HttpServletResponse& resp) {
    not_supported(req,resp,"GET");
}
//--------------------------------------------------------------------

void  HttpServlet::doHead(HttpServletRequest& req, HttpServletResponse& resp) {
    NoBodyResponse response(resp);
    doGet(req, response);
    response.setContentLength();
}
//--------------------------------------------------------------------

void  HttpServlet::doPost(HttpServletRequest& req, HttpServletResponse& resp) {
    not_supported(req,resp,"POST");
}
//--------------------------------------------------------------------

void  HttpServlet::doPut(HttpServletRequest& req, HttpServletResponse& resp) {
    not_supported(req,resp,"PUT");
}
//--------------------------------------------------------------------

void  HttpServlet::doDelete(HttpServletRequest& req, HttpServletResponse& resp) {
    not_supported(req,resp,"DELETE");
}
//--------------------------------------------------------------------
void  HttpServlet::doOptions(HttpServletRequest& req, HttpServletResponse& resp) {
    not_supported(req,resp,"OPTIONS");
/*      Method[] methods = getAllDeclaredMethods(this.getClass());

        boolean ALLOW_GET = false;
        boolean ALLOW_HEAD = false;
        boolean ALLOW_POST = false;
        boolean ALLOW_PUT = false;
        boolean ALLOW_DELETE = false;
        boolean ALLOW_TRACE = true;
        boolean ALLOW_OPTIONS = true;

        for (int i=0; i<methods.length; i++) {
            Method m = methods[i];

            if (m.getName() == ("doGet")) {
                ALLOW_GET = true;
                ALLOW_HEAD = true;
            }
            if (m.getName() == ("doPost"))
                ALLOW_POST = true;
            if (m.getName() == ("doPut"))
                ALLOW_PUT = true;
            if (m.getName() == ("doDelete"))
                ALLOW_DELETE = true;

        }

        String allow = null;
        if (ALLOW_GET)
            if (allow==null) allow=METHOD_GET;
        if (ALLOW_HEAD)
            if (allow==null) allow=METHOD_HEAD;
            else allow += ", " + METHOD_HEAD;
        if (ALLOW_POST)
            if (allow==null) allow=METHOD_POST;
            else allow += ", " + METHOD_POST;
        if (ALLOW_PUT)
            if (allow==null) allow=METHOD_PUT;
            else allow += ", " + METHOD_PUT;
        if (ALLOW_DELETE)
            if (allow==null) allow=METHOD_DELETE;
            else allow += ", " + METHOD_DELETE;
        if (ALLOW_TRACE)
            if (allow==null) allow=METHOD_TRACE;
            else allow += ", " + METHOD_TRACE;
        if (ALLOW_OPTIONS)
            if (allow==null) allow=METHOD_OPTIONS;
            else allow += ", " + METHOD_OPTIONS;

        resp.setHeader("Allow", allow);*/
}
//--------------------------------------------------------------------

void  HttpServlet::doTrace(HttpServletRequest& req, HttpServletResponse& resp) {
    not_supported(req,resp,"TRACE");//TODO
/*
        int responseLength;

        String CRLF = "\r\n";
        String responseString = "TRACE "+ req.getRequestURI()+
            " " + req.getProtocol();

        Enumeration reqHeaderEnum = req.getHeaderNames();

        while( reqHeaderEnum.hasMoreElements() ) {
            String headerName = (String)reqHeaderEnum.nextElement();
            responseString += CRLF + headerName + ": " +
                req.getHeader(headerName);
        }

        responseString += CRLF;

        responseLength = responseString.length();

        resp.setContentType("message/http");
        resp.setContentLength(responseLength);
        ServletOutputStream out = resp.getOutputStream();
        out.print(responseString);
        out.close();
        */
}
//--------------------------------------------------------------------
void  HttpServlet::service(HttpServletRequest& req, HttpServletResponse& resp) {
    const string method = req.getMethod();
    if(method == "GET") {
        long lastModified = getLastModified(req);
        if (lastModified == -1) {
            // servlet doesn't support if-modified-since, no reason
            // to go through further expensive logic
            doGet(req, resp);
        } else {
            long ifModifiedSince = req.getDateHeader("If-Modified-Since");
            if (ifModifiedSince < (lastModified / 1000 * 1000)) {
                // If the servlet mod time is later, call doGet()
                // Round down to the nearest second for a proper compare
                // A ifModifiedSince of -1 will always be less
                maybeSetLastModified(resp, lastModified);
                doGet(req, resp);
            } else {
                resp.setStatus(HttpServletResponse::SC_NOT_MODIFIED);
            }
        }
    }else if(method == "POST") {
            doPost(req, resp);
    }else if(method == "HEAD") {
        long lastModified = getLastModified(req);
        maybeSetLastModified(resp, lastModified);
        doHead(req, resp);
    }else if(method == "PUT") {
            doPut(req, resp);
    }else if(method == "DELETE") {
            doDelete(req, resp);
    }else if(method == "OPTIONS") {
            doOptions(req,resp);
    }else if(method == "TRACE") {
            doTrace(req,resp);
    }else {
            // Note that this means NO servlet supports whatever
            // method was requested, anywhere on this server.
            resp.sendError(
                HttpServletResponse::SC_NOT_IMPLEMENTED,
                "Method "+req.getMethod()+" not implemented"
            );
    }
}
//--------------------------------------------------------------------

void  HttpServlet::maybeSetLastModified(HttpServletResponse& resp, long lastModified) {
    if (resp.containsHeader("Last-Modified"))
        return;
    if (lastModified >= 0)
        resp.setDateHeader("Last-Modified", lastModified);
}
//--------------------------------------------------------------------

void  HttpServlet::service(ServletRequest& req, ServletResponse& res) {
    HttpServletRequest &request =
        dynamic_cast<HttpServletRequest&>(req);
    HttpServletResponse &response =
        dynamic_cast<HttpServletResponse&>(res);
    service(request, response);
}
//--------------------------------------------------------------------
