package store;

import store.cart_item;
import store.Product;
import store.Mysql;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import javax.servlet.ServletException;
import java.util.ArrayList;
import java.util.TreeMap;
import java.io.IOException;
import java.sql.ResultSet;
import java.sql.Connection;
import java.sql.SQLException;

public class ProductDetails extends HttpServlet {
    private Connection connection;

    void add_to_cart(HttpServletRequest request, HttpServletResponse response) throws IOException, SQLException {
        HttpSession session = request.getSession(false);
        if(session==null) throw new RuntimeException("no session");
        ArrayList cart = (ArrayList) session.getAttribute("cart");
        if(cart == null)
            cart = new ArrayList();
        cart_item item = new cart_item();
        item.quantity = Integer.parseInt(request.getParameter("quantity"));
        item.product = fetch(request.getParameter("product_id"));
        if(item.quantity <= 0)
            throw new RuntimeException("Quantity is invalid. It must be a positive number");
        cart.add(item);

        session.setAttribute("cart",cart);
        response.sendRedirect("ShoppingCart.sxx");
    }
    Product fetch(final String id) throws SQLException {
        final String sql = "SELECT * FROM store_products WHERE product_id=$id";
        TreeMap params = new TreeMap();
        params.put("id",id);

        ResultSet result = Mysql.query(connection, Mysql.prepare(sql, params));
        if( result.next() ) {
            Product product = new Product();
            product.product_id = Long.parseLong(result.getString(1));
            product.category_id = Long.parseLong(result.getString(2));
            product.name = result.getString(3);
            product.price = Float.parseFloat(result.getString(4));
            product.img = result.getString(5);
            product.desc = result.getString(6);
            product.recomended = Boolean.getBoolean(result.getString(7));
            return product;
        }else
            throw new RuntimeException("product_id "+id+" not found");
    }
    void show_product(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException, SQLException {
        String id = request.getParameter("product_id");
        if(id==null)
            throw new RuntimeException("ProductDetails: product_id parameter not supplyed");
        Product product = fetch(id);
        request.setAttribute("product", product);
        request.getRequestDispatcher("ProductDetailsView.jsp").forward(request,response);
    }
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        try{
            try{
                add_to_cart(request, response);
            }catch(Throwable ex) {
                ex.printStackTrace();
                request.setAttribute("error",ex.getMessage());
                show_product(request, response);
            }
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
            request.setAttribute("product",new Product());
            request.getRequestDispatcher("ProductDetailsView.jsp").forward(request,response);
        }
    }
/*

    public void init() throws ServletException {
        connection = Mysql.create();
    }

    public void destroy() {
        Mysql.destroy(connection);
    }
*/

    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        connection = Mysql.get();
        try{
            request.getSession(true); //start session
            show_product(request, response);
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
            request.setAttribute("product",new Product());
            request.getRequestDispatcher("ProductDetailsView.jsp").forward(request,response);
        }
    }
};