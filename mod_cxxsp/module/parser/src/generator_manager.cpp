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

#include "generator_manager.h"
#include <iostream>
using namespace std;

void generator_manager::load(const string& prefix, const string& uri) {
    cout << "generator_manager::load" <<endl;
    cout << "registry.size(): " << registry.size() << endl;
    if(registry.find(prefix) == registry.end()) {
        entry e;
        e.dso = new dso_code(uri);
        registry[prefix] = e;
    }
}

Generator& generator_manager::get(const string& prefix, const string& name) {
    cout << "generator_manager::get"  << endl;
    registry_t::iterator r = registry.find(prefix);
    if(r == registry.end())
        throw runtime_error("Prefix "+prefix+" not registered");
    entry e = r->second;
    if(!e.dso->reload_if_needed()) {
        map<string,Generator*>::iterator tag = e.tags.find(name);
        if(tag != e.tags.end())
            return *tag->second;
    }
    cout << "creating: " << name << endl;
    Generator* generator = e.dso->call<Generator*>("create_"+name);
    e.tags[name] = generator;
    return *generator;
}

generator_manager::~generator_manager(){
    for(registry_t::iterator lib = registry.begin(); lib != registry.end(); ++lib) {
        entry& e = lib->second;
        for(tags_t::iterator tag = e.tags.begin(); tag != e.tags.end(); ++tag) {
            delete tag->second;
            tag->second = NULL;
        }
        delete e.dso;
        e.dso = NULL;
    }
}
