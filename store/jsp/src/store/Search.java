package store;

import store.Category;
import store.Mysql;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;
import java.util.ArrayList;
import java.sql.ResultSet;
import java.sql.Connection;
import java.sql.SQLException;
import java.io.IOException;

public class Search extends HttpServlet {
    private Connection connection;

    ArrayList fetch_categories(Connection connection) throws SQLException {
        ResultSet result = Mysql.query(connection, "SELECT * FROM store_categories");
        ArrayList categories = new ArrayList();
        while( result.next() ) {
            Category category = new Category();
            category.id=Long.parseLong(result.getString(1));
            category.name=result.getString(2);
            categories.add(category);
        }
        return categories;
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
        request.setAttribute("categories", new ArrayList());
        try{
            request.setAttribute("categories", fetch_categories(connection));
        }catch(Throwable ex) {
            ex.printStackTrace();
            request.setAttribute("error",ex.getMessage());
        }
        request.getRequestDispatcher("SearchView.jsp").forward(request,response);
    }
};
