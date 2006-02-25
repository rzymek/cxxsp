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

#include <PageContext.h>

//--------------------------------------------------------------------

 PageContext::PageContext() const
:PAGE_SCOPE(1),REQUEST_SCOPE(2),SESSION_SCOPE(3),APPLICATION_SCOPE(4),PAGE("javax.servlet.jsp.jspPage"),PAGECONTEXT("javax.servlet.jsp.jspPageContext"),REQUEST("javax.servlet.jsp.jspRequest"),RESPONSE("javax.servlet.jsp.jspResponse"),CONFIG("javax.servlet.jsp.jspConfig"),SESSION("javax.servlet.jsp.jspSession"),OUT("javax.servlet.jsp.jspOut"),APPLICATION("javax.servlet.jsp.jspApplication"),EXCEPTION("javax.servlet.jsp.jspException")
 {
    }
//--------------------------------------------------------------------

const BodyContent&  PageContext::pushBody() const {
        return null; // XXX to implement
    }
//--------------------------------------------------------------------

const ErrorData&  PageContext::getErrorData() const {
        return new ErrorData(
            (Throwable)getRequest().getAttribute( "javax.servlet.error.exception" ),
            ((Integer)getRequest().getAttribute(
                "javax.servlet.error.status_code" )).intValue(),
            (String)getRequest().getAttribute( "javax.servlet.error.request_uri" ),
            (String)getRequest().getAttribute( "javax.servlet.error.servlet_name" ) );
    }
//--------------------------------------------------------------------
