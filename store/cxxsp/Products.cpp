#include <boost/lexical_cast.hpp>
#include <RequestDispatcher.h>
#include <boost/shared_ptr.hpp>
#include "Product.h"
#include "Category.h"
#include "mysql.h"
#include <HttpServlet.h>
#include <math.h>//ceilf

using namespace std;
using boost::lexical_cast;
using boost::shared_ptr;
using namespace servlet;

class Products : public HttpServlet {
    typedef vector<Product> products_t;
    typedef std::vector<Category> categories_t;

    inline bool is(const Mysql::params_t::const_iterator& iter, const std::string& name) {
        return (iter->first==name && !iter->second.empty());
    }

    categories_t fetch_categories();
    string where_part(const Mysql::params_t& params);
    string order_part(const std::string& productsOrder, const std::string& productsDir);
    inline int getPageSize(const std::string& productsPageSize);
    string limit_part(const std::string& productsPageSize, const std::string& productsPage);
    products_t fetch(const std::string& sql);
    int count(const std::string& sql);
    string make_naviLink(HttpServletRequest& request);
    void process(HttpServletRequest& request, HttpServletResponse &response);
public:
    void doGet(HttpServletRequest& request, HttpServletResponse &response);
};


Products::categories_t Products::fetch_categories() {
    const string sql = "SELECT * FROM store_categories";
    shared_ptr<MYSQL_RES> result = mysql.query(sql);

    categories_t categories;
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(result.get())) ) {
        Category category;
        category.id=lexical_cast<long>(mysql.get(row,0));
        category.name=mysql.get(row,1);
        categories.push_back(category);
    }
    return categories;
}

string Products::where_part(const Mysql::params_t& params) {
    vector<string> cond;
    for(map<string,string>::const_iterator iter = params.begin(); iter!=params.end(); ++iter) {
        if(is(iter,"category_id"))
            cond.push_back("category_id=$category_id");
        else if(is(iter,"s_keyword"))
            cond.push_back("product_name LIKE '%$s_keyword%'");
        else if(is(iter,"price_from"))
            cond.push_back("price >= $price_from");
        else if(is(iter,"price_to"))
            cond.push_back("price <= $price_to");
    }
    string query;
    if(!cond.empty()) {
        query += " WHERE ";
        for(unsigned i=0;;++i) {
            query += cond[i];
            if(i==cond.size()-1)
                break;
            query += " AND ";
        }
    }
    return mysql.prepare(query,params);
}
string Products::order_part(const std::string& productsOrder, const std::string& productsDir) {
    string query;
    if(!productsOrder.empty()) {
        query = " ORDER BY ";
        query += (productsOrder == "price") ? "price" : "product_name";
        if(!productsDir.empty()){
            query += (productsDir == "ASC") ? " ASC" : " DESC";
        }
    }
    return query;
}
int Products::getPageSize(const std::string& productsPageSize) {
    int pageSize = 10;
    if(!productsPageSize.empty())
        pageSize = lexical_cast<int>(productsPageSize);
    return pageSize;
}
string Products::limit_part(const std::string& productsPageSize, const std::string& productsPage) {
    string query = " LIMIT ";
    int pageSize = getPageSize(productsPageSize);
    query += lexical_cast<string>(pageSize);

    if(!productsPage.empty()) {
        int page = lexical_cast<int>(productsPage) - 1;
        if(page < 0)
            page = 0;
        query += " OFFSET " + lexical_cast<string>(page * pageSize);
    }
    return query;
}

Products::products_t Products::fetch(const std::string& sql) {
    shared_ptr<MYSQL_RES> result = mysql.query(sql);
    products_t products;
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(result.get())) ) {
        Product product;
        product.product_id = lexical_cast<long>(mysql.get(row,0));
        product.category_id = lexical_cast<long>(mysql.get(row,1));
        product.name = mysql.get(row,2);
        product.price = lexical_cast<float>(mysql.get(row,3));
        product.img = mysql.get(row,4);
        product.desc = mysql.get(row,5);
        product.recomended = lexical_cast<bool>(mysql.get(row,6));
        products.push_back(product);
    }
    return products;
}

int Products::count(const std::string& sql) {
    shared_ptr<MYSQL_RES> result = mysql.query(sql);
    MYSQL_ROW row;
    if( (row = mysql_fetch_row(result.get())) )
        return lexical_cast<int>(mysql.get(row,0));
    return 0;
}

string Products::make_naviLink(HttpServletRequest& request) {
    auto_ptr< vector<string> > names_ptr = request.getParameterNames();
    vector<string>& names = *names_ptr;
    string link;
    for(unsigned i=0;i<names.size(); ++i) {
        if(names[i] == "productsPage")
            continue;
        link += link.empty()?'?':'&';
        link += names[i] + "=" + request.getParameter(names[i]);
    }
    link += (link.empty()?'?':'&') + string("productsPage=");
    return ("Products.sxx"+link);

}

void Products::process(HttpServletRequest& request, HttpServletResponse &response) {
    map<string,string> params;
    params["category_id"]=request.getParameter("category_id");
    params["s_keyword"]=request.getParameter("s_keyword");
    params["price_from"]=request.getParameter("price_from");
    params["price_to"]=request.getParameter("price_to");

    const string WHERE = where_part(params) ;
    string query = "SELECT * FROM store_products"
        + WHERE
        + order_part(request.getParameter("productsOrder"), request.getParameter("productsDir"))
        + limit_part(request.getParameter("productsPageSize"), request.getParameter("productsPage"));

    products_t products = fetch(query);
    request.setAttribute("products", products);

    query = "SELECT COUNT(*) FROM store_products" + WHERE;
    int ResultsFound = count(query);
    request.setAttribute("ResultsFound", ResultsFound);

    int pageSize = getPageSize(request.getParameter("productsPageSize"));
    int pageCount = static_cast<int>( ceilf(static_cast<float>(ResultsFound)/pageSize) );
    request.setAttribute("pageCount", pageCount);

    int currentPage = 1;
    string s_currentPage = request.getParameter("productsPage");
    if(!s_currentPage.empty())
        currentPage = lexical_cast<int>(s_currentPage);
    request.setAttribute("currentPage", currentPage);

    request.setAttribute("naviLink",make_naviLink(request));
}

void Products::doGet(HttpServletRequest& request, HttpServletResponse &response) {
    try{
        request.setAttribute("categories", fetch_categories());
        process(request, response);
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
        request.setAttribute("ResultsFound", 0);
        request.setAttribute("products", products_t());
        request.setAttribute("naviLink",string());
        request.setAttribute("currentPage", 0);
    }
    request.getRequestDispatcher("ProductsView.csp")->forward(request,response);
}


EXPORT_SERVLET(Products);
