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

class DefaultGenerator : public TagGenerator {
public
        DefaultGenerator(const std::string& prefix, const std::string& name);
        void setAttribs(const attribs_t& attribs);
        void setParent(TagGenerator* gen);
        void start(std::ostream& out);
        void end(std::ostream& out);

        std::string tag_var_name;
};

/*
void for_each::start(ostream& out){
    string c = attribs["container"];
    for({type}::const_iterator {iter} = {cont}.begin(); {iter}!={cont}.end(); ++{iter}) {


    out << "
};
*/
void DefaultGenerator::start(ostream& out) {
        string tag = prefix+"_"+name;

        string eval = "_eval_"+tag;
        string set_attributes;
        for(attribs_t::const_iterator it=attribs.begin(); it!=attribs.end(); ++it) {
            set_attributes += "\t" + tag + ".setAttribute(\""
                + prefix + "\",\""
                + it->first + "\", \""
                + it->second + "\");\n";
        }


        string templ = "                                        \n\
        Tag& {tag} = _tagpool.aquire(\"{prefix}\", \"{name}\"); \n\
        {tag}.setParent({parent});                              \n\
        {set_attributes}                                        \n\
        Tag::StartTagResult {eval} = {tag}.doStartTag();        \n\
        if({eval} != Tag::SKIP_BODY) {                          \n\
            stringbuf _newbuf;                                  \n\
            streambuf* _oldbuf = out.rdbuf();                   \n\
            if ({eval} == Tag::EVAL_BODY_CONTENT) {             \n\
                out.rdbuf(&_newbuf);                            \n\
                {tag}.setBodyBuffer(&_newbuf);                  \n\
                {tag}.doInitBody();                             \n\
            }                                                   \n\
            for(;;){";

        fill(templ, "prefix", prefix);
        fill(templ, "name", name);
        fill(templ, "tag", tag);
        fill(templ, "eval", eval);
        if(parent == NULL)
                parent_tag = "NULL";
        else
                parent_tag = parent->tag_var_name;
        fill(templ, "parent", parent_tag);
        fill(templ, "set_attributes", set_attributes);

//         parent_tag = tag;

        code(templ);
};

