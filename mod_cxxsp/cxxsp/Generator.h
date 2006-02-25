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

#ifndef TAG_GENERATOR_H
#define TAG_GENERATOR_H

#include <iostream>
#include <map>
#include <string>

class Generator {
protected:
    std::ostream *body, *header, *member;
    Generator* parent;
public:
    typedef std::map<std::string, std::string> attribs_t;

    virtual ~Generator()
        { }
    virtual void setBuffers(std::ostream& body, std::ostream& header, std::ostream& member)
        { this->body = &body; this->header = &header; this->member = &member; }
    virtual void doStartTag(const attribs_t& attribs) = 0;
    virtual void doEndTag() = 0;
    virtual void setParent(Generator* gen)
        { parent = gen; }
};


#endif//GENERATOR_H

