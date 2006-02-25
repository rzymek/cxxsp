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

#include <stdexcept>
#include "parser.h"
#include "parser_error.h"
#include <utils/dir.h> //directory(...)

namespace cxxsp {

    using namespace std;

    parser::parser(const string& a_file_in, const string& a_file_out)
        : lex(open(a_file_in)), generate(a_file_out), file_in(a_file_in)
    {
    }

    istream& parser::open(const std::string file_in) {
        in = new ifstream();
        in->exceptions(ios::badbit);
        in->open(file_in.c_str());
        if(!in->is_open())
            throw not_found_error("Cannot open file: " + file_in);
        return *in;
    }

    string parser::get_data() {
        string data=lex.last_text();
        while(lex.peek_token() == lexer::CSP_DATA) {
            lex.next_token();
            data += lex.last_text();
        }
        return data;
    }

    void parser::get_token(lexer::token expected){
        lexer::token got = lex.next_token();
        if(got != expected)
            throw parser_error("Unexpected token:"+lex.last_text(),lex.lineno(), got,expected);
    }

    void parser::get_token(lexer::token expected1, lexer::token expected2){
        lexer::token got = lex.next_token();
        if(got != expected1 && got != expected2)
            throw parser_error("Unexpected token",lex.lineno(), got,expected1,expected2);
    }

    void parser::get_data(const string& expected){
        string got = get_data();
        if(expected != got)
            throw parser_error("Unexpected data. \nGot: '"+ got+"'\nExpected: '"+expected+"'",lex.lineno());
    }

    string parser::get_code() {
        stringstream data;

        //data << "\n#line " << lex.lineno()-1 << " \"" << file_in << "\"\n";

        data << lex.last_text();
        while(lex.peek_token() != lexer::CSP_CODE_END && lex.peek_token() != lexer::CSP_EOF) {
            lex.next_token();
            data << lex.last_text();
        }
        lex.next_token(); //eat CSP_CODE_END
        return data.str();
    }

   generator::attribs_t parser::get_tag_attribs(){
        generator::attribs_t attribs;
        string name,value;
        while(lex.peek_token() != lexer::CSP_SPECIAL_TAG_CLOSED && lex.peek_token() != lexer::CSP_EOF) {
            get_token(lexer::CSP_ID);
            name = lex.last_text();

            get_token(lexer::CSP_DATA);
            get_data("=");

            get_token(lexer::CSP_VALUE,lexer::CSP_ID);
            value = lex.last_text();

            attribs[name] = value;
        }
        lex.next_token();
        return attribs;
    }

    string parser::relative_path(const string& filename) {
        if(filename.empty())
            return filename;
        if(filename[0] == '/') {
            throw runtime_error("TODO: including relative to web aplication root");
        }else
            return directory(file_in) + "/" + filename;
    }

    void parser::parse() {
//         dbg_stop();

        for(;;){
            lexer::token token = lex.next_token();
            switch(token) {
            case lexer::CSP_EOF:
                generate.generate();
                return;
            case lexer::CSP_DATA:
                generate.data(get_data());
                break;
            case lexer::CSP_CODE_START:
                generate.code(get_code());
                break;
            case lexer::CSP_EXPR_START:
                generate.expr(get_code());
                break;
            case lexer::CSP_DECL_START:
                generate.decl(get_code());
                break;
            case lexer::CSP_HEADER_START:
                generate.header(get_code());
                break;
            case lexer::CSP_DIRECTIVE_START:{
                get_token(lexer::CSP_ID);
                string name = lex.last_text();
                generator::attribs_t attribs = get_tag_attribs();

                if(name == "include") {
                    generator::attribs_t::const_iterator attrib
                        = attribs.find("file");
                    if(attrib != attribs.end()){
                        lex.include(relative_path(attrib->second));
                    }else
                        throw parser_error("include directive without file attribute", lex.lineno());
                }else
                    generate.directive(name, attribs);
                break;
            }
            case lexer::CSP_LIBTAG_START:{
                string prefix = lex.last_text();
                get_token(lexer::CSP_ID);
                string name = lex.last_text();
                generate.libtag_start(prefix, name, get_tag_attribs());
                break;
            }
            case lexer::CSP_LIBTAG_STOP:{
                string prefix = lex.last_text();
                get_token(lexer::CSP_ID);
                string name = lex.last_text();
                get_token(lexer::CSP_SPECIAL_TAG_CLOSED);
                generate.libtag_end(prefix, name);
                break;
            }
            default:
                throw parser_error("Internal parser error",lex.lineno(), token);
            }
        }
    }

}//namespace
