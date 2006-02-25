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

#ifndef TOCKENIZER_H
#define TOCKENIZER_H

#include <string>

class tokenizer {
    const std::string& data;
    char sep;
    std::string::size_type start;
public:
    tokenizer(const std::string& the_data, char a_sep)
        : data(the_data)
        { start = 0; sep = a_sep; }
    bool end()
        { return start == std::string::npos; }
    std::string next() {
        std::string::size_type begin = start;
        start = data.find(sep, start);
        if(std::string::npos == start)
            return data.substr(begin);
        return data.substr(begin, start++ - begin);
    }
};

#endif//TOCKENIZER_H

