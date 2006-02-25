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

#include "Generator.h"
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

inline string fill(string tmpl, const Generator::attribs_t& attribs) {
    for(string::size_type i=0; i<tmpl.size(); ++i){
        if(tmpl[i]=='$') {
            string::size_type j;
            for(j = i+1; j<tmpl.size() ; ++j) {
                if(isalnum(tmpl[j]) || tmpl[j]=='_')
                    continue;
                break;
            }
            if(j>=tmpl.size())
                break;
            string var = tmpl.substr(i+1, j-(i+1));
            Generator::attribs_t::const_iterator val = attribs.find(var);
            if(val == attribs.end())
                throw runtime_error("Variable "+var+" not found in attribs");
            tmpl.replace(i, j-i, val->second);
            i = i + val->second.size();
        }
    }
    return tmpl;
}



class for_generator : public Generator{
public:
    void doStartTag(const attribs_t& attribs) {
        string tmpl = "\
        out << \"loop start  kkk    \" << std::endl;      \n\
        for(int $var=0;$var<$count;++$var) {    \n\
            int ii = $var*2;                    \n\
        ";
        *body << fill(tmpl,attribs) << endl;
    }

    void doEndTag() {
        *body << "}" << endl;
    }
};

#include <sstream>

class for_each_g : public Generator {
    stringstream func;
    streambuf* buf;
    attribs_t attr;
public:
    void setBuffers(std::ostream& body, std::ostream& header, std::ostream& member) {
        Generator::setBuffers(body, header, member);
    }
    void doStartTag(const attribs_t& attribs) {
        attr = attribs;
        string tmpl = "\n\
            template<typename I>                                            \n\
            struct _for_each_functor : public std::unary_function<double, I> {   \n\
                std::ostream& out;                                          \n\
                _for_each_functor(std::ostream& a_out) : out(a_out) { }     \n\
                void operator()(I $arg) {                                   \n";
        func << fill(tmpl, attribs);
        buf = body->rdbuf();
        body->rdbuf(func.rdbuf());
    }
    void doEndTag() {
        body->rdbuf(buf);
        func << "}\n};   \n\
            template<typename T>    \n\
            _for_each_functor<T> _make(T, std::ostream& out) {    \n\
                return _for_each_functor<T>(out);                 \n\
            }                                                       \n"
            << endl;
        *header << "\n#include <algorithm>\n" << endl;
        *header << "\n#include <functional>\n" << endl;
        *member << func.str() << endl;
        string tmpl = "\n\
            for_each($first, $last, _make(*$first, out));  \n";
        *body << fill(tmpl, attr) << endl;
    }
};

extern "C" Generator* create_for() { return new for_generator(); }
extern "C" Generator* create_for_each() { return new for_each_g(); }
