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
#include <mysql/mysql.h>

using namespace std;
using namespace servlet;

class Test2 : public HttpServlet {
    MYSQL *mysql;
public:
    void init() {
        mysql = mysql_init(NULL);
        mysql_real_connect(mysql, "localhost","","","internet",3306,"/var/run/mysqld/mysqld.sock",0);
    }
    void doGet(const HttpServletRequest& request, HttpServletResponse &response) {
        //dbg_stop();

        response.setContentType("text/html");
        ostream& out = response.getOutputStream();
        out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\""
            "\"http://www.w3.org/TR/html4/loose.dtd\">" << endl;
        out << "<html><head><title>test</title></head><body>" << endl;

        out << "   mysql_query:" << mysql_query(mysql, "SELECT * FROM store_products") << endl;
        out << "<table border>" << endl;
        MYSQL_RES *result = mysql_use_result(mysql);
        MYSQL_ROW row;
        unsigned num_fields = mysql_num_fields(result);
        //cout << "   num_fields = " <<num_fields <<endl;
        while((row = mysql_fetch_row(result))) {
            out << "<tr>" << endl;

//             long product_id = dbi_result_get_long(result, "product_id");
//             long category_id = dbi_result_get_long(result, "category_id");
//             string name = dbi_result_get_string(result, "product_name");
//             float price = dbi_result_get_float(result, "price");
//             string img = dbi_result_get_string(result, "image_url");
//             string desc = dbi_result_get_string(result, "description");
//             bool recomended = dbi_result_get_long(result, "is_recommended")==1;

            for(unsigned i=0;i<num_fields;++i)
            out << "<td>" << (row[i]?row[i]:"NULL") << "</td>" <<endl;
/*            out << "<td>" << price << "</td>" <<endl;
            out << "<td>" << img << "</td>" <<endl;
            out << "<td>" << desc << "</td>" <<endl;
            out << "<td>" << recomended << "</td>" <<endl;*/
            out << "</tr>" << endl;
        }
        out << "</table>" << endl;
        out << "</body></html>" << endl;
        out << " mysql_errno(mysql):"<<mysql_errno(mysql)<<endl;
        mysql_free_result(result);
    }

    void destory() {
      mysql_close(mysql);
    }
};

extern "C" HttpServlet* create() { return new Test2(); }
