INCLUDE=-I../../mod_servlet/servlet/include/ -I$(BOOST_INC)

all: libstoremysql.so ProductDetails.sxx ShoppingCart.sxx Search.sxx Products.sxx Default.sxx 

%.sxx: %.cpp
	$(CXX) -o $@ $^ $(INCLUDE) -shared -L/usr/lib/mysql -lmysqlclient -L$(BOOST_LIB) -lboost_date_time -L . -lstoremysql

libstoremysql.so: mysql.o
	$(CXX) -o $@ $^ $(INCLUDE) -shared -L/usr/lib/mysql -lmysqlclient 

%.o: %.cpp
	$(CXX) -c -o $@ $^ $(INCLUDE) 

deploy: install
install: all
	cp libstoremysql.so $(APACHE_HOME)/lib
	mkdir -p $(HTDOCS)/store/csp/style
	mkdir -p $(HTDOCS)/store/csp/images/books
	rm -f $(HTDOCS)/store/csp/*.sxx 
	cp *.sxx *.csp *.h $(HTDOCS)/store/csp/
	cp style/* $(HTDOCS)/store/csp/style
	cp images/*.*  $(HTDOCS)/store/csp/images
	cp images/books/*.* $(HTDOCS)/store/csp/images/books
clean:
	rm -f *.o *.sxx *.so
	rm -rf $(APACHE_HOME)/cxxsp/*
