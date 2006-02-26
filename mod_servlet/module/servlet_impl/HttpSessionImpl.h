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
#ifndef HTTPSESSIONIMPL_H
#define HTTPSESSIONIMPL_H

#include <boost/serialization/access.hpp>
#include <servlet/HttpSession.h>
#include <servlet/ServletContext.h>
#include "../src/ipr.h"

class HttpSessionImpl : public servlet::HttpSession {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & is_new;
        ar & created;
        ar & maxInactiveInterval;
    }

    std::string id;
    long created, accessed, maxInactiveInterval;
public:
    mutable ipr::client *ipr_client;
    HttpSessionImpl(const std::string& id);

    long  getCreationTime() const;
    std::string  getId() const;
    long  getLastAccessedTime() const;

    servlet::ServletContext& getServletContext() const;
    std::string __getAttributeImpl(const std::string& name) const;
    void __setAttributeImpl(const std::string& name, const std::string& value);
    void setMaxInactiveInterval(int interval);
    int  getMaxInactiveInterval() const;
    bool hasAttribute(const std::string& name) const;
    std::auto_ptr< std::vector<std::string> > getAttributeNames() const;
    void removeAttribute(const std::string& name);

    void invalidate();

    bool is_new;
    bool isNew() const;
};

#endif//HTTPSESSIONIMPL_H

