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
#include <servlet/HttpServlet.h>
#include <time.h>

using namespace std;
using namespace servlet;

class SimpleServlet : public HttpServlet {
public:

    void doGet(HttpServletRequest& request, HttpServletResponse &response) {
        response.setContentType("text/html");
        ostream& out = response.getOutputStream();
        out << "<html><body>" << endl;
        HttpSession* session = request.getSession();
        if(session->hasAttribute("lastVisit")) {
            time_t lastVisit = session->getAttribute<time_t>("lastVisit");
            out << "Witaj ponownie. Pierwszy raz odwiedziles nas: <br>"
                << ctime(&lastVisit) << endl;
        }else{
            out << "Witaj po raz pierszy!" << endl;
            time_t lastVisit = time(NULL);
            session->setAttribute("lastVisit", lastVisit);
        }
        out << "</body></html>" << endl;
    }
};

EXPORT_SERVLET(SimpleServlet);
