OBJS=obj/NoBodyResponse.o obj/HttpServlet.o obj/HttpServletRequest.o obj/GenericServlet.o obj/HttpServletResponse.o obj/Cookie.o  
INCLUDE=-Iinclude -Isrc -I$(BOOST_INC) -I$(APACHE_HOME)/include -I../../common
LIBS=
TARGET=libservlet.a 
#TARGET=libservlet.a libservlet.so

all: obj/ obj/boost/ $(TARGET) 
install: all
#	cp libservlet.so $(APACHE_HOME)/lib

libservlet.a: $(OBJS)
	ar cr $@ $(OBJS) obj/boost/*
libservlet.so: $(OBJS) 
	$(CXX) -o $@ -shared $^ $(LIBS) obj/boost/*
obj/boost/:	
	mkdir obj/boost
	cd obj/boost && ar x $(BOOST_LIB)
obj/:
	mkdir obj
obj/%.o: src/%.cpp 
	$(CXX) -o $@ -c $(INCLUDE) $<
clean:
	rm -rf obj/ $(TARGET)
