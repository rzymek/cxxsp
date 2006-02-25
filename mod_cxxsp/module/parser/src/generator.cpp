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

#include "generator.h"
#include <stdexcept>
#include <fstream>
#include <boost/tokenizer.hpp>
#include "tokenizer.h"
#include "Generator.h"
#include <utils/text.h>

namespace cxxsp {

    using namespace std;

    inline string generate_class_name(const string& filename) {
        string class_name = "CspServlet";
        for(string::size_type i=0;i<filename.size();++i){
            if(!isalpha(filename[i]))
                class_name += '_';
            else
                class_name += filename[i];
        }
        return class_name;
    }

    generator::generator(const string& filename) {
        session = false;
        isErrorPage = false;
        class_name = generate_class_name(filename);
        this->filename = filename;
        contentType = "text/html ; charset=ISO-8859-1";
        extends = "servlet::HttpServlet";

        head << "#include <HttpServlet.h>" << endl;
    }

    string generator::escape(const string& s){
        string res;
        res.reserve(s.size() + s.size()/3);
        for(unsigned i=0;i<s.size();++i){
            switch(s[i]){
                case '\\':
                    res += "\\\\";
                    break;
                case '\r':
                    res += "\\r";
                    break;
                case '\n':
                    res += "\\n";
                    break;
                case '"':
                    res += "\\\"";
                    break;
                default:
                    res += s[i];
            }
        }
        return res;
    }

    void generator::data(const string& data) {
         if(!data.empty())
            body << "\tout.write(\"" << escape(data) << "\", " << data.size() << ");\n";
    }

    void generator::decl(const string& data) {
        head << data << endl;
    }

    void generator::code(const string& data){
        body << data << endl;
    }

    void generator::expr(const string& data){
        body << "out << ( " << data << ");\n";
    }

    void generator::header(const string& data){
        head << data << endl;
    }

    inline void fill(string &templ, string name, string val){
        string::size_type pos;
        const string needle = "{" + name + "}";
        for(;;){
            pos = templ.find(needle);
            if(pos == string::npos)
                break;
            templ.replace(pos, needle.size(), val);
        }
    }

    void generator::libtag_start(const string& prefix, const string& name, const attribs_t& attribs) {
        Generator& gen = generators.get(prefix,name);
        gen.setBuffers(body,head,member);
        gen.doStartTag(attribs);
        gen.setParent(generators.top());//TODO
        generators.push(gen);

/*        DefaultGenerator* gen = new DefaultGenerator(prefix,name);
        if(!tag_gen.empty())
                gen->setParent(tag_gen.top());
        gen->setAttribs(attribs);
        tag_gen.push(gen);
        gen.start(body);*/
    }

    void generator::libtag_end(const string& prefix, const string& name){
        Generator& gen = generators.pop();
        gen.doEndTag();

/*        string eval = "_eval_"+tag;

        string templ = "                                        \n\
                if ({tag}.doAfterBody() != Tag::EVAL_BODY_AGAIN)\n\
                    break;                                      \n\
            }                                                   \n\
            if ({eval} == Tag::EVAL_BODY_CONTENT){              \n\
                out.rdbuf(_oldbuf);                             \n\
                out << _newbuf.str();                           \n\
            }                                                   \n\
        }                                                       \n\
        if({tag}.doEndTag() == Tag::SKIP_PAGE)                  \n\
            return;                                             \n\
        _tagpool.release({tag}); ";

        fill(templ, "tag", tag);
        fill(templ, "eval", eval);

        code(templ);*/
    }


    void generator::directive(const string& name, const attribs_t& attribs){
        /* include directive is processed by lexer*/
        if(name == "page") {
            attribs_t::const_iterator attrib;

            attrib = attribs.find("language");
            if(attrib != attribs.end())
                if(attrib->second != "c++")
                    throw runtime_error("Invalid language. Only accepted value is \"c++\"");

            attrib = attribs.find("import");
            if(attrib != attribs.end()) {
                tokenizer tok(attrib->second,',');
                while(!tok.end())
                    head << "#include <" << tok.next() << ">" << endl;
            }

            attrib = attribs.find("session");
            if(attrib != attribs.end())
                session = (attrib->second == "true");

            attrib = attribs.find("isErrorPage");
            if(attrib != attribs.end())
                isErrorPage = (attrib->second == "true");

            attrib = attribs.find("errorPage");
            if(attrib != attribs.end()) {
                errorPage = attrib->second;
                if(errorPage == "this")
                    errorPage = filename;
            }
            attrib = attribs.find("contentType");
            if(attrib != attribs.end())
                contentType = attrib->second;

            attrib = attribs.find("extends");
            if(attrib != attribs.end())
                extends = attrib->second;

            attrib = attribs.find("flags");
            if(attrib != attribs.end())
                split_to(attrib->second, flags_);

        }else if(name == "taglib"){
            attribs_t::const_iterator attrib;
            string prefix,uri;

            attrib = attribs.find("uri");
            if(attrib != attribs.end()){
                uri = attrib->second;
            }else throw runtime_error("uri attribute missing from taglib directive");

            attrib = attribs.find("prefix");
            if(attrib != attribs.end()){
                prefix = attrib->second;
            }else throw runtime_error("prefix attribute missing from taglib directive");

            generators.load(prefix, uri);
        }else{
            throw runtime_error("Unknown directive: " + name);
        }
    }

    void generator::generate() {
        ofstream out;
        out.exceptions(ios::badbit | ios::failbit);
        out.open(filename.c_str());

        if(session)
            out << "#include <memory>\n";
        out << head.str() << endl
            << "class " << class_name << ": public " << extends << " {"
            << member.str() << endl

            << "public:\n"
            << "    void doPost(servlet::HttpServletRequest& request, servlet::HttpServletResponse& response) {\n"
            << "        doGet(request, response);\n"
            << "    }\n"
            << "    void doGet(servlet::HttpServletRequest& request, servlet::HttpServletResponse& response) {\n";
        if(isErrorPage) {
            out << "        const std::exception* exception = request.hasAttribute(\"csp.exception\") ? \n"
                << "            &request.getAttribute<std::exception>(\"csp.exception\") : \n"
                << "            0;\n";
        }
        if(!errorPage.empty())
            out << "    try { \n";
        out << "        std::ostream& out = response.getOutputStream();\n"
            << "        response.setContentType(\"" << escape(contentType) <<"\");\n";
        if(session)
            out << "        servlet::HttpSession* session = request.getSession();\n";
        out << endl
            << body.str() << endl;
        if(!errorPage.empty()) {
            out << "    }catch(const std::exception& e) { \n"
                << "        request.setAttribute(\"csp.exception\", e);\n"
                << "        if(exception == 0) /* avoid exception loop */ { "
                << "            request.getRequestDispatcher(\"" << errorPage << "\")->forward(request,response); \n"
                << "        } else {\n"
                << "            throw;"
                << "        }\n"
                << "    }\n";
        }
        out << "    }\n"
            << "};\n"
            << "EXPORT_SERVLET(" << class_name << ");" << endl;
        out.close();
    }

}
