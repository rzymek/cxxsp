package store;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

public class Default extends HttpServlet {
    private Connection connection;

    ArrayList fetch_categories() throws SQLException {
        final String sql = "SELECT * FROM store_categories";

        ArrayList categories = new ArrayList();
        Statement statement = null;
        ResultSet result = null;
        try {
            statement = connection.createStatement();
            result = statement.executeQuery(sql);
            while( result.next() ) {
                Category category = new Category();
                category.id = Long.parseLong(result.getString(1));
                category.name=result.getString(2);
                categories.add(category);
            }
        }finally{
            Mysql.free(result,statement);
        }
        return categories;
    }        
    ResultSet articles_query_product(final String category) throws SQLException {
        final String sql = "SELECT art.article_title, art.article_desc, prod.product_id, prod.image_url FROM " +
            " store_editorial_products as art, store_products as prod WHERE " +
            " art.editorial_cat_id="+category+" and art.product_id=prod.product_id";
        return Mysql.query(connection, sql);
    }
    ResultSet articles_query(final String category) throws SQLException {
        final String sql = "SELECT art.article_title, art.article_desc FROM " +
            " store_editorial_products as art WHERE " +
            " art.editorial_cat_id="+category;
        return Mysql.query(connection, sql);
    }
    ArrayList fetch_articles(final String category) throws SQLException {
        ResultSet result = articles_query(category);
        ArrayList articles = new ArrayList();
        while( result.next() ) {
            Article article = new Article();
            article.title = result.getString(1);
            article.desc = result.getString(2);
            articles.add(article);
        }
        return articles;
    }
    ArrayList fetch_articles_product(final String category) throws SQLException {
        ResultSet result = articles_query_product(category);
        ArrayList articles = new ArrayList();
        while( result.next() ) {
            Article article = new Article();
            article.title = result.getString(1);
            article.desc = result.getString(2);
            article.link = "ProductDetails.sxx?product_id=" + result.getString(3);
            article.img = result.getString(4);
            
            articles.add(article);
        }
        return articles;
    }
    ArrayList fetch_products(Connection connection) throws SQLException {
        ResultSet result = Mysql.query(connection, "SELECT * FROM store_products WHERE is_recommended");
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
            request.setAttribute("weekly", fetch_articles("4"));
            request.setAttribute("featured", fetch_articles_product("3"));
            request.setAttribute("wereReading", fetch_articles_product("1"));
            request.setAttribute("notable", fetch_articles_product("2"));
            request.setAttribute("recomended", fetch_products(connection));
        }catch(Throwable ex){
            ex.printStackTrace();
            request.setAttribute("error", ex.getMessage());
        } 
        request.getRequestDispatcher("DefaultView.jsp").forward(request,response);
    }
};
