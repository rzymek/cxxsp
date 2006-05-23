OBJS=obj/dir.o obj/http.o
INCLUDE=-Iinclude -Isrc -I$(BOOST_INC) -I. -I$(APACHE_HOME)/include 
APACHE_LIB_DIR=$(APACHE_HOME)/lib
LIBS=
TARGET=libcxxsp-common.a libcxxsp-common.so

all: obj/ $(TARGET) 
install: libcxxsp-common.so
	cp $^ $(APACHE_LIB_DIR)
libcxxsp-common.a: $(OBJS)
	ar cr $@ $(OBJS) 
libcxxsp-common.so: $(OBJS) 
	$(CXX) -o $@ -shared $^ $(LIBS) 
obj/:
	mkdir obj
obj/%.o: src/%.cpp 
	$(CXX) -o $@ -c $(INCLUDE) $<
clean:
	rm -rf obj/ $(TARGET)
