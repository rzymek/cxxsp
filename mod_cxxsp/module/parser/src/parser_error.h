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

#ifndef PARSER_ERROR_H
#define PARSER_ERROR_H

#include <stdexcept>
#include <string>
#include <sstream>
#include "lexer.h"
#include  <boost/lexical_cast.hpp>

namespace cxxsp{

    class parser_error : public std::runtime_error {
    public:
        parser_error(const std::string& msg, int lineno)
            : std::runtime_error(msg+"\nLine: "+boost::lexical_cast<std::string>(lineno))
            { }
        parser_error(const std::string& msg, int lineno, lexer::token token)
            : std::runtime_error(msg+"\nLine: "+boost::lexical_cast<std::string>(lineno)+"\nGot: "+token_string(token))
            { }
        parser_error(const std::string& msg, int lineno, lexer::token token,lexer::token exp)
            : std::runtime_error(msg+"\nLine: "+boost::lexical_cast<std::string>(lineno)+"\nGot: "+token_string(token)+". Expected: "+token_string(exp))
            { }
        parser_error(const std::string& msg, int lineno, lexer::token token,lexer::token exp1,lexer::token exp2)
            : std::runtime_error(msg+"\nLine: "+boost::lexical_cast<std::string>(lineno)+"\nGot: "+token_string(token)+
                ". Expected: "+token_string(exp1)+" or "+token_string(exp2))
            { }

    };

}
#endif//PARSER_ERROR_H

