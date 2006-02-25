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

#ifndef GENERATOR_MANAGER_H
#define GENERATOR_MANAGER_H
#include <string>
#include <stack>
#include <map>
#include <string>
#include <stdexcept>
#include "../cxxsp/Generator.h"
#include "../../mod_servlet/module/src/dso_code.h"

class generator_manager {
    typedef std::map<std::string, Generator*> tags_t;

    std::stack<Generator*> generators;

    struct entry {
        dso_code* dso;
        tags_t tags;
    };

    typedef std::map<std::string, entry> registry_t;
    registry_t registry;
public:
    ~generator_manager();
    void load(const std::string &prefix, const std::string &uri);
    Generator& get(const std::string& prefix, const std::string& name);
    Generator* top()
        { return (generators.empty() ? NULL : generators.top()); }
    Generator& pop()
        { Generator& top = *generators.top(); generators.pop(); return top; }
    void push(Generator& g)
        { generators.push(&g); }
};


#endif//GENERATOR_MANAGER_H

