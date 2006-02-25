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

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>

#ifndef yyFlexLexer
    #define yyFlexLexer CxxspFlexLexer
    #include <FlexLexer.h>
#endif

#define MAX_INCLUDE_DEPTH 100

namespace cxxsp{

    class lexer : protected CxxspFlexLexer {
    public:
        enum token {
            CSP_EOF = 0,
            CSP_CODE_START = 2,
            CSP_EXPR_START = 3,
            CSP_DECL_START = 4,
            CSP_HEADER_START = 5,
            CSP_DIRECTIVE_START = 6,
            CSP_LIBTAG_START = 7,
//             CSP_LIBTAG_END = 15,

            CSP_LIBTAG_STOP = 9,
            CSP_SPECIAL_TAG_CLOSED = 10,
            CSP_DATA = 11 ,
            CSP_CODE_END = 12,
            CSP_VALUE = 13,
            CSP_ID = 14
        };
    private:
        struct include_stack_t {
            yy_buffer_state* state;
            std::ifstream* in;
        };

        include_stack_t include_stack[MAX_INCLUDE_DEPTH];
        int include_stack_ptr;

        std::string text,last;

        token current;

        void save_text(int start_strip=0, int end_strip=0);
        void emit_line();

        int yylex();
        bool eof(); //returns false if it's and eof of an included file (so not a real eof)

        yy_buffer_state* get_current_buffer();
        static int buffer_size;
        void terminate();
    public:
        lexer(std::istream& in);

        /* The text of the last token (as returned by next_token()) */
        std::string last_text()
            { return last; }
        token next_token();
        token peek_token()
            { return current; }

        int lineno() const {
            return CxxspFlexLexer::lineno();
        }

        void include(const std::string& filename);
    }; //class

    std::string token_string(lexer::token token);
} //namespace
#endif//LEXER_H

