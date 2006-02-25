#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <RequestDispatcher.h>
#include <vector>
#include "Category.h"
#include "mysql.h"
#include <HttpServlet.h>
using namespace servlet;

class Search : public HttpServlet {
protected:
    typedef std::vector<Category> categories_t;
    categories_t fetch_categories();
public:
    void doGet(HttpServletRequest& request, HttpServletResponse &response);
};

using namespace std;
using boost::shared_ptr;
using boost::lexical_cast;

Search::categories_t Search::fetch_categories() {
    shared_ptr<MYSQL_RES> result = mysql.query("SELECT * FROM store_categories");
    MYSQL_ROW row;
    categories_t categories;
    while( (row = mysql_fetch_row(result.get())) ) {
        Category category;
        category.id=lexical_cast<long>(mysql.get(row,0));
        category.name=mysql.get(row,1);
        categories.push_back(category);
    }
    return categories;
}

void Search::doGet(HttpServletRequest& request, HttpServletResponse &response) {
    request.setAttribute("categories", categories_t());
    try{
        request.setAttribute("categories", fetch_categories());
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
    }
    request.getRequestDispatcher("SearchView.csp")->forward(request,response);
}

EXPORT_SERVLET(Search);
