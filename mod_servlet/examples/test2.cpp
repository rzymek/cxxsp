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
#include <dbi/dbi.h>

using namespace std;
using namespace servlet;

class Test2 : public HttpServlet {
    dbi_conn conn;
public:
    void init() {
        cout << "   dbi_initialize: " << dbi_initialize("/usr/lib/dbd/") << endl;
        dbi_driver d = dbi_driver_list(NULL);
        cout << "   d="<<d<<endl;
        cout << dbi_driver_get_name(d) << endl;
        conn = dbi_conn_new("mysql");
        if(conn == NULL) {
            throw runtime_error("dbi_conn_new error1");
        }
        dbi_conn_set_option(conn, "host", "localhost");
        dbi_conn_set_option(conn, "dbname", "internet");
        cout << "   dbi_conn_connect: " << dbi_conn_connect(conn) << endl;

    }
    void doGet(const HttpServletRequest& request, HttpServletResponse &response) {
        //dbg_stop();

        response.setContentType("text/html");
        ostream& out = response.getOutputStream();
        out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\""
            "\"http://www.w3.org/TR/html4/loose.dtd\">" << endl;
        out << "<html><head><title>test</title></head><body>" << endl;

        out << "<table border>" << endl;
        dbi_result result = dbi_conn_query(conn, "SELECT * FROM store_products");
        while(dbi_result_next_row(result)) {
            out << "<tr>" << endl;

            long product_id = dbi_result_get_long(result, "product_id");
            long category_id = dbi_result_get_long(result, "category_id");
            string name = dbi_result_get_string(result, "product_name");
            float price = dbi_result_get_float(result, "price");
            string img = dbi_result_get_string(result, "image_url");
            string desc = dbi_result_get_string(result, "description");
            bool recomended = dbi_result_get_long(result, "is_recommended")==1;

            out << "<td>" << product_id << "</td>" <<endl;
            out << "<td>" << category_id << "</td>" <<endl;
            out << "<td>" << name << "</td>" <<endl;
            out << "<td>" << price << "</td>" <<endl;
            out << "<td>" << img << "</td>" <<endl;
            out << "<td>" << desc << "</td>" <<endl;
            out << "<td>" << recomended << "</td>" <<endl;
            out << "</tr>" << endl;
        }
        out << "</table>" << endl;
        out << "</body></html>" << endl;

        dbi_result_free(result);
    }

    void destory() {
        dbi_conn_close(conn);
        dbi_shutdown();
    }
};

extern "C" HttpServlet* create() { return new Test2(); }
