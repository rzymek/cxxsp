#include "mysql.h"
#include <HttpServlet.h>
#include <HttpSession.h>
#include <RequestDispatcher.h>
#include "cart_item.h"
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;
using namespace boost;
using namespace servlet;

class ShoppingCart : public HttpServlet {
    typedef vector<cart_item> cart_t;

    void edit_cart(HttpServletRequest& request);
    void purchase(HttpServletRequest& request);
    void calc_total(HttpServletRequest& request);
public:
    void doPost(HttpServletRequest& request, HttpServletResponse& response);
    void doGet(HttpServletRequest& request, HttpServletResponse& response);
};

//------------------------------------------------------------------------------------

void ShoppingCart::edit_cart(HttpServletRequest& request){
    const string del_op = "delete_";
    if(request.hasAttribute("cart")) {
        cart_t cart = request.getAttribute<cart_t>("cart");
        string si,param;
        long product_id;
        for(unsigned i=0; i<cart.size(); ++i) {
            si = lexical_cast<string>(i);
            param = del_op + si;
            if(!request.getParameter(param).empty()) {
                product_id = lexical_cast<long>(request.getParameter("product_id_" + si));
                if(cart[i].product.product_id == product_id)//additional check
                    cart[i].quantity = 0;
                continue;
            }
            param = "quantity_" + si;
            cart[i].quantity = lexical_cast<int>(request.getParameter(param));
            if(cart[i].quantity <= 0)
                throw runtime_error("Quantity is invalid. It must be a positive number");
        }
        cart.erase(
            std::remove_if(cart.begin(), cart.end(), lambda::_1 == 0),
            cart.end()
        ); //smart, ain't it? PS. _1 is from boost::lambda, see http://boost.org/doc/html/lambda.html#introduction
        request.getSession()->setAttribute("cart", cart);
        request.setAttribute("cart",cart);
    }
}

void ShoppingCart::purchase(HttpServletRequest& request){
    try{
        mysql.execute("START TRANSACTION");
        string sql = "INSERT INTO store_orders (order_date, order_status_id, total) VALUES ('$date',1,$total)";
        Mysql::params_t params;
        params["date"] = to_iso_string(posix_time::second_clock::local_time());
        params["total"] = lexical_cast<string>(request.getAttribute<float>("total"));
        mysql.execute(mysql.prepare(sql,params));
        params.clear();

        my_ulonglong order_id = mysql_insert_id(mysql.handle);
        params["order_id"] = lexical_cast<string>(order_id);
        cart_t cart = request.getAttribute<cart_t>("cart");
        for(unsigned i=0; i<cart.size(); ++i) {
            sql = "INSERT INTO store_order_items (order_id, product_id, quantity, price ) "
                "VALUES ($order_id, $product_id, $quantity, $price)";
            params["product_id"] = lexical_cast<string>(cart[i].product.product_id);
            params["quantity"] = lexical_cast<string>(cart[i].quantity);
            params["price"] = lexical_cast<string>(cart[i].product.price); //if the prices changes later
            mysql.execute(mysql.prepare(sql,params));
        }

        HttpSession *session = request.getSession(false);
        if(session) {
            session->removeAttribute("cart");
            request.setAttribute("cart", cart_t());
        }

        mysql.execute("COMMIT");
    }catch(...){
        mysql.execute("ROLLBACK");
        throw;
    }
}

void ShoppingCart::calc_total(HttpServletRequest& request){
    float total = 0;
    if(request.hasAttribute("cart")) {
        cart_t cart = request.getAttribute<cart_t>("cart");
        for(unsigned i=0;i<cart.size(); ++i)
            total += (cart[i].quantity * cart[i].product.price);
    }
    request.setAttribute("total", total);
}

void ShoppingCart::doPost(HttpServletRequest& request, HttpServletResponse& response) {
    try{
        request.setAttribute(
            "cart",
            request.getSession()->getAttribute<cart_t>("cart")
        );
        if(request.getParameter("form") == "cart_edit")  {
            edit_cart(request);
            calc_total(request);
        } else if(request.getParameter("form") == "purchase") {
            calc_total(request);
            purchase(request);
            response.sendRedirect("PayFlow.csp");
            return;
        }else
            calc_total(request);
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
        if(!request.hasAttribute("cart"))
            request.setAttribute("cart", cart_t());
        if(!request.hasAttribute("total"))
            request.setAttribute<float>("total", -1.0f);
    }
    request.getRequestDispatcher("ShoppingCartView.csp")->forward(request,response);
}

void ShoppingCart::doGet(HttpServletRequest& request, HttpServletResponse& response){
    try{
        request.setAttribute(
            "cart",
            request.getSession(false)->getAttribute<cart_t>("cart")
        );
        calc_total(request);
    }catch(const exception& ex) {
        request.setAttribute<string>("error",ex.what());
        if(!request.hasAttribute("cart"))
            request.setAttribute("cart", cart_t());
        if(!request.hasAttribute("total"))
            request.setAttribute<float>("total", -1.0f);
    }
    request.getRequestDispatcher("ShoppingCartView.csp")->forward(request,response);
}


EXPORT_SERVLET(ShoppingCart);
