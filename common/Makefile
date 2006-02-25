OBJS=obj/dir.o obj/http.o
INCLUDE=-Iinclude -Isrc -I$(BOOST_INC) -I. -I$(APACHE_HOME)/include 
APACHE_LIB_DIR=$(APACHE_HOME)/lib
LIBS=
TARGET=libcommon.a libcommon.so

all: obj/ $(TARGET) 
install: libcommon.so
	cp libcommon.so $(APACHE_LIB_DIR)
libcommon.a: $(OBJS)
	ar cr $@ $(OBJS) 
libcommon.so: $(OBJS) 
	$(CXX) -o $@ -shared $^ $(LIBS) 
obj/:
	mkdir obj
obj/%.o: src/%.cpp 
	$(CXX) -o $@ -c $(INCLUDE) $<
clean:
	rm -rf obj/ $(TARGET)
