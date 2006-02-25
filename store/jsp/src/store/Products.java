package store;

import store.Category;
import store.Mysql;
import store.Product;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;
import java.util.*;
import java.sql.ResultSet;
import java.sql.Connection;
import java.sql.SQLException;
import java.io.IOException;

public class Products extends HttpServlet {
    private Connection connection;

    boolean is(final Map.Entry iter, final String name) {
        return (iter.getKey().equals(name) && iter.getValue()!=null);
    }
        
    ArrayList fetch_categories() throws SQLException {
        final String sql = "SELECT * FROM store_categories";
        ResultSet result = Mysql.query(connection, sql);
    
        ArrayList categories = new ArrayList();
        while( result.next() ) {
            Category category = new Category();
            category.id=Long.parseLong(result.getString(1));
            category.name=result.getString(2);
            categories.add(category);
        }
        return categories;
    }        

    String where_part(final Map params) {
        ArrayList cond = new ArrayList();
        for(Iterator iter = params.entrySet().iterator(); iter.hasNext(); ) {
            final Map.Entry entry = (Map.Entry) iter.next();
            if(is(entry,"category_id"))
                cond.add("category_id=$category_id");
            else if(is(entry,"s_keyword"))
                cond.add("product_name LIKE '%$s_keyword%'");
            else if(is(entry,"price_from"))
                cond.add("price >= $price_from");
            else if(is(entry,"price_to"))
                cond.add("price <= $price_to");
        }
    	String query = "";
        if(!cond.isEmpty()) {
            query += " WHERE ";
            for(int i=0;;++i) {
                query += cond.get(i);
                if(i==cond.size()-1)
                    break;
                query += " AND ";
            }
        }
        return Mysql.prepare(query,params);
    }
    String order_part(final String productsOrder, final String productsDir) {
        String query="";
        if(productsOrder!=null) {
            query = " ORDER BY ";
            query += (productsOrder.equals("price")) ? "price" : "product_name";
            if(productsDir != null){
                query += (productsDir.equals("ASC")) ? " ASC" : " DESC";
            }
        }
        return query;
    }
    int getPageSize(final String productsPageSize) {
        int pageSize = 10;
        if(productsPageSize!=null)
            pageSize = Integer.parseInt(productsPageSize);
        return pageSize;
    }
    String limit_part(final String productsPageSize, final String productsPage) {
        String query = " LIMIT ";
        int pageSize = getPageSize(productsPageSize);
        query += String.valueOf(pageSize);

        if(productsPage!=null) {
            int page = Integer.parseInt(productsPage) - 1;
            if(page < 0) 
                page = 0;
            query += " OFFSET " + String.valueOf(page * pageSize);
        }  
        return query;
    }
    
    ArrayList fetch(final String sql) throws SQLException {
        ResultSet result = Mysql.query(connection, sql);
        ArrayList products = new ArrayList();
        while( result.next() ) {
            Product product = new Product();
            product.product_id = Long.parseLong(result.getString(1));
            product.category_id = Long.parseLong(result.getString(2));
            product.name = result.getString(3);
            product.price = Float.parseFloat(result.getString(4));
            product.img = result.getString(5);
            product.desc = result.getString(6);
            product.recomended = Boolean.getBoolean(result.getString(7));
            products.add(product);
        }
        return products;
    }

    int count(final String sql) throws SQLException {
        ResultSet result = Mysql.query(connection, sql);
        if( result.next() )
            return Integer.parseInt(result.getString(1));
        return 0;
    }
    
    String make_naviLink(HttpServletRequest request) {
        Enumeration names = request.getParameterNames();
        StringBuffer link= new StringBuffer();
        while (names.hasMoreElements()) {
            String name = (String) names.nextElement();
            if(name == "productsPage")
                continue;
            link.append((link.length()==0)?'?':'&');
            link.append(name).append("=").append(request.getParameter(name));
        }
        link.append((link.length()==0)?'?':'&').append("productsPage=");
        return ("Products.sxx"+link);
        
    }
    
    void process(HttpServletRequest request, HttpServletResponse response) throws SQLException {
        TreeMap params = new TreeMap();
        params.put("category_id",request.getParameter("category_id"));
        params.put("s_keyword", request.getParameter("s_keyword"));
        params.put("price_from", request.getParameter("price_from"));
        params.put("price_to", request.getParameter("price_to"));
        
        final String WHERE = where_part(params) ;
        String query = "SELECT * FROM store_products"
            + WHERE
            + order_part(request.getParameter("productsOrder"), request.getParameter("productsDir"))
            + limit_part(request.getParameter("productsPageSize"), request.getParameter("productsPage"));
	
        ArrayList products = fetch(query);
        request.setAttribute("products", products);
        
        query = "SELECT COUNT(*) FROM store_products" + WHERE;
        int ResultsFound = count(query);
        request.setAttribute("ResultsFound", new Integer(ResultsFound));
        
        int pageSize = getPageSize(request.getParameter("productsPageSize"));
        int pageCount = (int) Math.ceil(((float)ResultsFound)/pageSize);
        request.setAttribute("pageCount", new Integer(pageCount));
        
        int currentPage = 1;
        String s_currentPage = request.getParameter("productsPage");
        if(s_currentPage!=null)
            currentPage = Integer.parseInt(s_currentPage);
        request.setAttribute("currentPage", new Integer(currentPage));
        
        request.setAttribute("naviLink",make_naviLink(request));
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
            request.setAttribute("categories", fetch_categories());
            process(request, response);
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
            request.setAttribute("ResultsFound", new Integer(0));
            request.setAttribute("products", new ArrayList());
            request.setAttribute("naviLink","");
            request.setAttribute("currentPage", new Integer(0));
        }
        request.getRequestDispatcher("ProductsView.jsp").forward(request,response);
    }
};
