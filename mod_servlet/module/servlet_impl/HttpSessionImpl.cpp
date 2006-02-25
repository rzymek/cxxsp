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
#include "HttpSessionImpl.h"
#include <stdexcept>
#include <apr_time.h>

using namespace std;
using namespace servlet;

HttpSessionImpl::HttpSessionImpl(const string& aid) : id(aid)
{
    created = static_cast<long>(apr_time_now());
    accessed = created;
}
//----------------------------------------------------------------------------
ServletContext&  HttpSessionImpl::getServletContext() const{
    throw runtime_error("TODO: getServletContext() const");
}
//----------------------------------------------------------------------------
void  HttpSessionImpl::invalidate() {
/*    vector<string> names = getAttributeNames();
    for(unsigned i=0; i<names.size(); ++i)
        removeAttribute(names[i]);
    shared_repository::instance().remove(id,"");*/
    throw runtime_error("TODO HttpSessionImpl::invalidate");

}
//----------------------------------------------------------------------------
long  HttpSessionImpl::getCreationTime() const {
    return created;
}
string HttpSessionImpl::getId() const {
    return id;
}
long  HttpSessionImpl::getLastAccessedTime() const {
    return accessed;
}

string HttpSessionImpl::__getAttributeImpl(const string& name) const {
    ipr_client->open(id);
    return ipr_client->get(name);
}

void HttpSessionImpl::__setAttributeImpl(const string& name, const string& value) {
    ipr_client->open(id);
    ipr_client->set(name, value);
}

void HttpSessionImpl::setMaxInactiveInterval(int interval) {
    maxInactiveInterval = interval;
}
int HttpSessionImpl::getMaxInactiveInterval() const {
    return maxInactiveInterval;
}

bool HttpSessionImpl::hasAttribute(const string& name) const {
    ipr_client->open(id);
    return ipr_client->has(name);
}

auto_ptr< vector<string> > HttpSessionImpl::getAttributeNames() const {
    ipr_client->open(id);
    return ipr_client->get_names();
}

void HttpSessionImpl::removeAttribute(const string& name) {
    ipr_client->open(id);
    ipr_client->remove(name);
}

bool HttpSessionImpl::isNew() const {
    return is_new;
}
