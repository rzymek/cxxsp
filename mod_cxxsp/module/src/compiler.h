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

#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include <utils/text.h>

namespace cxxsp {

    class compiler{
        std::vector<std::string> includes_, default_flags_;
    public:
        compiler() 
            { include("."); }
        void compiler_main(const std::string& cpp_path, const std::string& out_path, const std::vector<std::string>& params);
        void process(const std::string& source_path, const std::string& out_path, const std::vector<std::string>& flags);
        void include(const std::string& path) 
            { includes_.push_back(path); }
        void default_flags(const std::string& flags) 
            { split_to(flags, default_flags_); }
    };

}
#endif//COMPILER_H

