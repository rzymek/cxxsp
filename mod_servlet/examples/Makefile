INCLUDE=-I../servlet/include/ -I$(BOOST_INC)
HTDOCS=$(APACHE_HOME)/htdocs/

#all: Manager.sxx simple.sxx
all: test.sxx Manager.sxx simple.sxx noop.sxx
#all: test.sxx test2.sxx mysql.sxx Manager.sxx simple.sxx

mysql.sxx: mysql.cpp
	$(CXX) -o $@ $^ $(INCLUDE) -shared -L/usr/lib/mysql -lmysqlclient
Manager.sxx: Manager.cpp
	$(CXX) -o $@ $^ $(INCLUDE) -shared -I$(APACHE_HOME)/include
%.sxx: %.cpp
	$(CXX) -o $@ $^ $(INCLUDE) -shared

deploy: install

$(HTDOCS)/servletxx:
	mkdir $@
install: all $(HTDOCS)/servletxx/
	rm -f $(HTDOCS)/servletxx/*.sxx
	cp *.sxx $(HTDOCS)/servletxx/
clean:
	rm -f *.o *.sxx
