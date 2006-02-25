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

#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iostream>
#include "generator_manager.h"

namespace cxxsp {

    class generator {
        std::stringstream head,member,body;
        std::string escape(const std::string& s);
        std::string class_name, filename;

        bool session, isErrorPage;
        std::string errorPage;
        std::string contentType, extends;
        std::vector<std::string> flags_;

        generator_manager generators;
    public:
        generator(const std::string& filename);
        typedef std::map<std::string, std::string> attribs_t;

        void data(const std::string& data);
        void decl(const std::string& data);
        void code(const std::string& data);
        void expr(const std::string& data);
        void header(const std::string& data);
        void libtag_start(const std::string& prefix, const std::string& name, const attribs_t& attribs);
        void libtag_end(const std::string& prefix, const std::string& name);
        void directive(const std::string& name, const attribs_t& attribs);

        void generate();
        const std::vector<std::string>& flags() 
            { return flags_; }    
    };

}

#endif//GENERATOR_H

