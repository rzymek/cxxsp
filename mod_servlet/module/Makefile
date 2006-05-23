OBJS=obj/mod_servlet.o obj/engine.o obj/loader.o obj/ServletRequestImpl.o obj/ServletResponseImpl.o obj/HttpServletRequestImpl.o obj/HttpServletResponseImpl.o obj/HttpSessionImpl.o obj/dso_code.o obj/RequestDispatcherImpl.o obj/unix_socket.o obj/ipr.o
INCLUDE=-Isrc -Iservlet_impl -I$(APACHE_HOME)/include -I../servlet/include -I $(BOOST_INC) -I../../common
#LIBS=$(BOOST_LIB) -L../../common -lcommon 
LIBS=$(BOOST_LIB) ../servlet/obj/*.o  ../../common/libcxxsp-common.a 
#-L../../common -lcxxsp-common 
#LIBS=$(BOOST_INC)/libs/iostreams/src/file_descriptor.o  -L../../common -lcommon ../servlet/obj/*.o ../servlet/obj/boost/*.o
TARGET=mod_servlet.so
NAME=servlet
APXS=$(APACHE_HOME)/bin/apxs

all: obj/ $(TARGET)

install: all
	$(APXS) -a -i -n $(NAME) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LIBS) 
obj/:
	mkdir obj
obj/%.o: servlet_impl/%.cpp
	$(CXX) -o $@ -c $(INCLUDE) $<
obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $(INCLUDE) $<
clean:
	rm -rf obj/ mod_servlet.so
