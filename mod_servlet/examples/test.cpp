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
#include <HttpServlet.h>

#include <stdexcept>
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace servlet;

class Test : public HttpServlet {
public:
    void doPost(HttpServletRequest& req, HttpServletResponse &res) {
        doGet(req,res);
    }
    void doGet(HttpServletRequest& req, HttpServletResponse &res) {
        //dbg_stop();

        res.setContentType("text/html");
        ostream& out = res.getOutputStream();
        out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\""
            "\"http://www.w3.org/TR/html4/loose.dtd\">" << endl;
        out << "<html><head><title>test</title></head><body>" << endl;
        out << "PID: " << getpid() << "<hr>" << endl;

        auto_ptr< vector<string> > names = req.getHeaderNames();

        out << "<div style='border: solid black'>" << endl;
        out << "Headers:<ul>" << endl;
        for(unsigned i=0; i< names->size();++i) {
            out << "<li>" << (*names)[i] << "<ul>";
            auto_ptr< vector<string> > values = req.getHeaders((*names)[i]);
            for(unsigned j=0; j < values->size();++j) {
                out << "<li>" << (*values)[j] << "</li>";
            }
            out << "</ul>\n";
        }
        out << "</ul></div><br>\n";

        out << "<div style='border: solid black'>" << endl;
        out << "Cookies:<ul>" << endl;
        auto_ptr< vector<Cookie> > cookies = req.getCookies();
        for(unsigned i=0;i<cookies->size();++i)
            out << "<li><u>" <<  (*cookies)[i].getName() << "</u> "
                << (*cookies)[i].getValue() << "</li>\n" << endl;
        out << "</ul></div><br>\n";


        out << "<div style='border: solid black'>" << endl;
        out << "Parameters:<ul>" << endl;
        names = req.getParameterNames();
        for(unsigned i=0; i< names->size();++i) {
            out << "<li>" << (*names)[i] << "<ul>";
            auto_ptr< vector<string> > values = req.getParameterValues((*names)[i]);
            for(unsigned j=0; j < values->size();++j) {
                out << "<li>" << (*values)[j] << "</li>\n";
             //   Cookie c(names[i], req.getParameter(names[i]));
             //   res.addCookie(c);
            }
            out << "</ul>\n";
        }
        out << "</ul></div><br>";

        HttpSession* session = req.getSession();
        if(session){
            int cnt;
            if(session->isNew()){
                cnt = 1;
                session->setAttribute("cnt",cnt);
            }else{
                cnt = session->getAttribute<int>("cnt");
                cnt++;
                session->setAttribute("cnt",cnt);
            }
            out << "Your counter: " << cnt << endl;
            out << "<br>ID: " << session->getId() << endl;
        }else
            out << "<h4>No session</h4>";

        out << "</body></html>" << endl;
    }
};

EXPORT_SERVLET(Test);
