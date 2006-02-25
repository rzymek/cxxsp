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

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "lexer.h"
#include "generator.h"

namespace cxxsp {

    class parser {
        lexer lex;
        generator generate;

        std::string file_in;
        std::ifstream* in;
        std::istream& open(const std::string file_in);

        std::string get_data();
        /* Make the lexer get a next token.
         If the new token is not one the the 'expected' ones,
         then throw an exception.
        */
        void get_token(lexer::token expected);
        void get_token(lexer::token expected1, lexer::token expected2);

        /* Get next token. Assert it's CSP_DATA and that
        text read is equeal to 'expected' */
        void get_data(const std::string& expected);

        std::string get_code();
        generator::attribs_t get_tag_attribs();
        std::string relative_path(const std::string& filename);
    public:
        parser(const std::string& file_in, const std::string& file_out);
        ~parser()
            { in->close(); delete in; }
        void parse();
        const std::vector<std::string>& flags() 
            { return generate.flags(); }
    }; //class

    //-------------------------------------------------------------------------


} //namespace

#endif//PARSER_H

