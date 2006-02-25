package store;

import store.Mysql;
import store.cart_item;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;
import java.sql.Connection;
import java.sql.Date;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.TreeMap;
import java.io.IOException;

public class ShoppingCart extends HttpServlet {
    void edit_cart(HttpServletRequest request){
        final String del_op = "delete_";
        if(request.getAttribute("cart")!=null) {
            ArrayList cart = (ArrayList) request.getAttribute("cart");
            String si,param;
            long product_id;
            for(int i=0; i<cart.size(); ++i) {
                si = String.valueOf(i);
                param = del_op + si;
                if(request.getParameter(param)!=null) {
                    product_id = Long.parseLong(request.getParameter("product_id_" + si));
                    if(((cart_item)cart.get(i)).product.product_id == product_id)//additional check
                        ((cart_item)cart.get(i)).quantity = 0;
                    continue;
                }
                param = "quantity_" + si;
                ((cart_item)cart.get(i)).quantity = Integer.parseInt(request.getParameter(param));
                if(((cart_item)cart.get(i)).quantity <= 0)
                    throw new RuntimeException("Quantity is invalid. It must be a positive number");
            }
            for (Iterator iterator = cart.iterator(); iterator.hasNext();) {
                cart_item cart_item = (cart_item) iterator.next();
                if(cart_item.quantity==0)
                    iterator.remove();
            }
            request.getSession().setAttribute("cart", cart);
            request.setAttribute("cart",cart);
        }
    }
    
    void purchase(Connection connection, HttpServletRequest request) throws Throwable {
        try{
            Mysql.execute(connection, "START TRANSACTION");
            String sql = "INSERT INTO store_orders (order_date, order_status_id, total) VALUES ('$date',1,$total)";
            TreeMap params = new TreeMap();
            params.put("date", String.valueOf(new java.sql.Date(new java.util.Date().getTime())));
            params.put("total", String.valueOf(request.getAttribute("total")));
            Mysql.execute(connection, Mysql.prepare(sql,params));
            params.clear();

            ResultSet resultSet = Mysql.query(connection, "SELECT LAST_INSERT_ID()");
            resultSet.next();
            params.put("order_id", resultSet.getString(1));

            ArrayList cart = (ArrayList) request.getAttribute("cart");
            for(int i=0; i<cart.size(); ++i) {
                sql = "INSERT INTO store_order_items (order_id, product_id, quantity, price ) " +
                    "VALUES ($order_id, $product_id, $quantity, $price)";
                params.put("product_id", String.valueOf(((cart_item)cart.get(i)).product.product_id));
                params.put("quantity", String.valueOf(((cart_item)cart.get(i)).quantity));
                params.put("price", String.valueOf(((cart_item)cart.get(i)).product.price)); //if the prices changes later
                Mysql.execute(connection,  Mysql.prepare(sql,params));
            }        
            
            HttpSession session = request.getSession(false);
            if(session!=null) {
                session.removeAttribute("cart");
                request.setAttribute("cart", new ArrayList());
            }
            
            Mysql.execute(connection, "COMMIT");
        }catch(Throwable ex) {
            Mysql.execute(connection, "ROLLBACK");
            throw ex;
        }
    }
    
    void calc_total(HttpServletRequest request){
        float total = 0;
        if(request.getAttribute("cart")!=null) {
            ArrayList cart = (ArrayList) request.getAttribute("cart");
            for(int i=0;i<cart.size(); ++i)
                total += (((cart_item)cart.get(i)).quantity * ((cart_item)cart.get(i)).product.price);
        }
        request.setAttribute("total", new Float(total));
    }
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        Connection connection = Mysql.get();
        try{
            request.setAttribute(
                "cart", 
                request.getSession().getAttribute("cart")
            );
            if(request.getParameter("form").equals("cart_edit"))  {
                edit_cart(request);
                calc_total(request);
            } else if(request.getParameter("form").equals("purchase")) {
                calc_total(request);
                purchase(connection, request);
                response.sendRedirect("PayFlow.jsp");
                return;
            }else
                calc_total(request);
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
            if(request.getAttribute("cart")!=null)
                request.setAttribute("cart", new ArrayList());
            if(request.getAttribute("total")!=null)
                request.setAttribute("total", new Float(-1.0f));
        }
        request.getRequestDispatcher("ShoppingCartView.jsp").forward(request,response);
    }


    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        try{
            request.setAttribute(
                "cart", 
                request.getSession(false).getAttribute("cart")
            );
            calc_total(request); 
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
            if(request.getAttribute("cart")!=null)
                request.setAttribute("cart", new ArrayList());
            if(request.getAttribute("total")!=null)
                request.setAttribute("total", new Float(-1.0f));
        }
        request.getRequestDispatcher("ShoppingCartView.jsp").forward(request,response);
    }
};
