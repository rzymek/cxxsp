#include <servlet/HttpServlet.h>
#include <servlet/RequestDispatcher.h>
#include <boost/serialization/vector.hpp>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <mysql/mysql.h>

#include <algorithm>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include "cart_item.h"
#include "Product.h"
#include "mysql.h"

using namespace std;
using namespace servlet;

using boost::lexical_cast;
using boost::shared_ptr;

class ProductDetails : public HttpServlet {
    void add_to_cart(HttpServletRequest& request, HttpServletResponse &response);
    Product fetch(const std::string& id);
    void show_product(HttpServletRequest& request, HttpServletResponse &response);
public:
    void doPost(HttpServletRequest& request, HttpServletResponse &response);
    void doGet(HttpServletRequest& request, HttpServletResponse &response);
};

//-------------------------------------------------------------------------------

void ProductDetails::add_to_cart(HttpServletRequest& request, HttpServletResponse &response) {
    HttpSession* session = request.getSession(false);
    if(!session) throw runtime_error("no session");
    typedef vector<cart_item> shopping_cart;
    shopping_cart cart = session->getAttribute<shopping_cart>("cart");
    cart_item item;
    item.quantity = boost::lexical_cast<int>(request.getParameter("quantity"));
    item.product = fetch(request.getParameter("product_id"));
    if(item.quantity <= 0)
        throw runtime_error("Quantity is invalid. It must be a positive number");
    cart.push_back(item);

    session->setAttribute("cart",cart);
    response.sendRedirect("ShoppingCart.sxx");
}

Product ProductDetails::fetch(const std::string& id) {
    const string sql = "SELECT * FROM store_products WHERE product_id=$id";
    map<string,string> params;
    params["id"] = id;

    shared_ptr<MYSQL_RES> result = mysql.query(mysql.prepare(sql, params));
    MYSQL_ROW row;
    if( (row = mysql_fetch_row(result.get())) ) {
        Product product;
        product.product_id = lexical_cast<long>(mysql.get(row,0));
        product.category_id = lexical_cast<long>(mysql.get(row,1));
        product.name = mysql.get(row,2);
        product.price = lexical_cast<float>(mysql.get(row,3));
        product.img = mysql.get(row,4);
        product.desc = mysql.get(row,5);
        product.recomended = lexical_cast<bool>(mysql.get(row,6));
        return product;
    }else
        throw runtime_error("product_id "+id+" not found");
}

void ProductDetails::show_product(HttpServletRequest& request, HttpServletResponse &response) {
    string id = request.getParameter("product_id");
    if(id.empty())
        throw runtime_error("ProductDetails: product_id parameter not supplyed");
    Product product = fetch(id);
    request.setAttribute("product", product);
    request.getRequestDispatcher("ProductDetailsView.csp")->forward(request,response);
}

void ProductDetails::doPost(HttpServletRequest& request, HttpServletResponse &response) {
    try{
        try{
            add_to_cart(request, response);
        }catch(const exception& ex) {
            request.setAttribute<string>("error",ex.what());
            show_product(request, response);
        }
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
        request.setAttribute("product",Product());
        request.getRequestDispatcher("ProductDetailsView.csp")->forward(request,response);
    }
}

void ProductDetails::doGet(HttpServletRequest& request, HttpServletResponse &response) {
    try{
        request.getSession(true); //start session
        show_product(request, response);
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
        request.setAttribute("product",Product());
        request.getRequestDispatcher("ProductDetailsView.csp")->forward(request,response);
    }
}


EXPORT_SERVLET(ProductDetails);
