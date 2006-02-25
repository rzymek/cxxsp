#include <RequestDispatcher.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include "Product.h"
#include "Category.h"
#include "Article.h"
#include "mysql.h"
#include <HttpServlet.h>

using namespace std;
using boost::lexical_cast;
using boost::shared_ptr;
using namespace servlet;

class Default : public HttpServlet {
    typedef vector<Category> categories_t;
    typedef vector<Article> articles_t;
    typedef vector<Product> products_t;

    categories_t fetch_categories();
    shared_ptr<MYSQL_RES> articles_query_product(const string& category);
    shared_ptr<MYSQL_RES> articles_query(const string& category);
    articles_t fetch_articles(const string& category);
    articles_t fetch_articles_product(const string& category);
    products_t fetch_products();
public:
    void doGet(HttpServletRequest& request, HttpServletResponse &response);
};

//-----------------------------------------------------------------------------------


Default::categories_t Default::fetch_categories() {
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

shared_ptr<MYSQL_RES> Default::articles_query_product(const string& category) {
    const string sql = "SELECT art.article_title, art.article_desc, prod.product_id, prod.image_url FROM "
        " store_editorial_products as art, store_products as prod WHERE "
        " art.editorial_cat_id="+category+" and art.product_id=prod.product_id";
    return mysql.query(sql);
}
shared_ptr<MYSQL_RES> Default::articles_query(const string& category) {
    const string sql = "SELECT art.article_title, art.article_desc FROM "
        " store_editorial_products as art WHERE "
        " art.editorial_cat_id="+category;
    return mysql.query(sql);
}


Default::articles_t Default::fetch_articles(const string& category) {
    shared_ptr<MYSQL_RES> result = articles_query(category);
    articles_t articles;
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(result.get())) ) {
        Article article;
        article.title = mysql.get(row,0);
        article.desc = mysql.get(row,1);
        articles.push_back(article);
    }
    return articles;
}
Default::articles_t Default::fetch_articles_product(const string& category) {
    shared_ptr<MYSQL_RES> result = articles_query_product(category);
    articles_t articles;
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(result.get())) ) {
        Article article;
        article.title = mysql.get(row,0);
        article.desc = mysql.get(row,1);
        article.link = "ProductDetails.sxx?product_id=" + mysql.get(row,2);
        article.img = mysql.get(row,3);

        articles.push_back(article);
    }
    return articles;
}
Default::products_t Default::fetch_products() {
    shared_ptr<MYSQL_RES> result = mysql.query("SELECT * FROM store_products WHERE is_recommended");
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



void Default::doGet(HttpServletRequest& request, HttpServletResponse &response) {
    try{
        request.setAttribute("categories", fetch_categories());
        request.setAttribute("weekly", fetch_articles("4"));
        request.setAttribute("featured", fetch_articles_product("3"));
        request.setAttribute("wereReading", fetch_articles_product("1"));
        request.setAttribute("notable", fetch_articles_product("2"));
        request.setAttribute("recomended", fetch_products());
    }catch(const exception& ex){
        request.setAttribute<string>("error",ex.what());
    }
    request.getRequestDispatcher("DefaultView.csp")->forward(request,response);
}

EXPORT_SERVLET(Default);
