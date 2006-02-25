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

#ifndef MAP_H
#define MAP_H

#include <string>
#include <map>
#include <vector>
#include <memory>

template<typename mapT>
inline std::auto_ptr< std::vector<std::string> > get_keys(const mapT& smap){
    std::auto_ptr< std::vector<std::string> > names(new std::vector<std::string>());
    std::string last="";
    for(typename mapT::const_iterator iter = smap.begin(); iter != smap.end(); ++iter){
        if(iter->first != last){
            names->push_back(iter->first);
            last = iter->first;
        }
    }
    return names;
}

template<typename Key, typename Value>
inline Value get_value(const std::multimap<Key,Value>& smap, const Value& name){
    typename std::multimap<Key,Value>::const_iterator res = smap.find(name);
    return res == smap.end() ? Value() : res->second;
}

template<typename Key, typename Value>
inline std::auto_ptr< std::vector<Value> > get_values(const std::multimap<Key,Value>& smap, const Value& name){
    std::auto_ptr< std::vector<Value> > values(new std::vector<Value>());
    for(typename std::multimap<Key,Value>::const_iterator res = smap.find(name);
        res != smap.end() && res->first == name; ++res)
    {
        values->push_back(res->second);
    }
    return values;
}


#endif//MAP_H

