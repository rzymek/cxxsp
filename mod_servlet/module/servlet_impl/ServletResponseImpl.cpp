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

#include "ServletResponseImpl.h"

using namespace std;
using namespace servlet;

//----------------------------------------------------------------------------

ServletResponseImpl::ServletResponseImpl(request_rec* req)
        : output_stream(req),r(req)
{
}
//----------------------------------------------------------------------------
std::string ServletResponseImpl::getCharacterEncoding() {
    throw runtime_error("TODO: ServletResponseImpl::getCharacterEncoding()");
}
//----------------------------------------------------------------------------
std::string ServletResponseImpl::getContentType() {
    return content_type;
}
//----------------------------------------------------------------------------
std::ostream&  ServletResponseImpl::getOutputStream() {
    return output_stream;
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::setCharacterEncoding(const std::string& charset) {
    throw runtime_error("TODO: ServletResponseImpl::setCharacterEncoding(const std::string& charset)");
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::setContentLength(int len) {
    throw runtime_error("TODO: ServletResponseImpl::setContentLength(int len)");
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::setContentType(const std::string& type) {
    content_type = type;
    ap_set_content_type(r, content_type.c_str());
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::setBufferSize(int size) {
    throw runtime_error("TODO: ServletResponseImpl::setBufferSize(int size)");
}
//----------------------------------------------------------------------------
int  ServletResponseImpl::getBufferSize() {
    throw runtime_error("TODO: ServletResponseImpl::getBufferSize()");
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::flushBuffer() {
    output_stream.flush();
}
//----------------------------------------------------------------------------
bool  ServletResponseImpl::isCommitted()   {
    throw runtime_error("TODO: ServletResponseImpl::isCommitted()");
}
//----------------------------------------------------------------------------
void  ServletResponseImpl::reset() {
    throw runtime_error("TODO: ServletResponseImpl::reset()");
}
//----------------------------------------------------------------------------
/*
void ServletResponseImpl::setLocale(const Locale &locale)  {
    throw runtime_error("TODO: ServletResponseImpl::setLocale(const Locale &locale)");
}
//----------------------------------------------------------------------------
const Locale& ServletResponseImpl::getLocale() {
    throw runtime_error("TODO: ServletResponseImpl::getLocale()");
}
*/
//----------------------------------------------------------------------------
