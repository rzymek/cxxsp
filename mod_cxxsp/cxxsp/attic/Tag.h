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

#ifndef TAG_H
#define TAG_H

#include <stdexcept>
#include <string>

class Tag {
    Tag *parent;
public:
    enum StartTagResult {
        SKIP_BODY,
        EVAL_BODY_CONTENT,
        EVAL_BODY /* bodyContent not set */
    };
    enum EndTagResult {
        EVAL_PAGE,SKIP_PAGE
    };
    enum AfterBodyResult {
        END_EVAL_BODY, //SKIP_BODY
        EVAL_BODY_AGAIN
    };

    Tag() : parent(NULL)
        {  }
    virtual ~Tag()
        { }

    virtual void setParent(Tag* parent)
        { this->parent = parent; }
    Tag* getParent()
        { return parent; }


    virtual StartTagResult doStartTag()
        { return EVAL_BODY; }

    virtual EndTagResult doEndTag() {
        return EVAL_PAGE;
    }

    virtual void setAttribute(const std::string& uri, const std::string& name, const std::string& value)
        { throw std::runtime_error("Unknown attribute "+name); }

    virtual AfterBodyResult doAfterBody()
        { return END_EVAL_BODY; }

    virtual void doInitBody()
        { }

    virtual void setBodyBuffer(std::stringbuf* buf)
        { }

    typedef std::vector< std::pair<std::string, std::string> > variables_t;
    variables_t getVariables() {
        return variables_t();
    }
};
//--------------------------------------------------------------------------
#include <sstream>

    class for_each : public Tag{
    public:
        void setAttribute(const std::string& uri, const std::string& name, const std::string& value)
            { }
        EndTagResult doEndTag() {
            return EVAL_PAGE;
        }
    };

    class uppercase : public Tag {
        std::stringbuf *buf;
    public:
        StartTagResult doStartTag() {
            return EVAL_BODY_CONTENT;
        }
        void setBodyBuffer(std::stringbuf* buf) {
            this->buf = buf;
        }
        AfterBodyResult doAfterBody() {
            std::string s = buf->str();
            for(unsigned i=0;i<s.size();++i)
                s[i] = toupper(s[i]);
            buf->str(s);
            return END_EVAL_BODY;
        }
    };


class for_each : public TagGenerator {
public:
	for_each(const attrbutes_t& attribs);
	void start(ostream& out){
	};
	void end(ostream& out){
	};
};


class TagPool {
public:
    TagPool() {
    }
    Tag& aquire(const std::string& prefix, const std::string& name) {
        if(name == "for_each")
            return *new for_each();
        else if(name == "uppercase")
            return *new uppercase();
        throw std::runtime_error("Tag not found");
    }
    void release(const Tag& tag){
    }
};
#endif//TAG_H

