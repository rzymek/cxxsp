OBJS=obj/compiler.o obj/engine.o  obj/mod_cxxsp.o
INCLUDE=-Isrc -I$(APACHE_HOME)/include -I../../mod_servlet/servlet -I../cxxsp -I../../common
LIBS=../../common/libcxxsp-common.a
#LIBS=-L../../common -lcxxsp-common
TARGET=mod_cxxsp.so
NAME=cxxsp
APXS=$(APACHE_HOME)/bin/apxs

all: obj/ $(TARGET) parser/parser.a

install: all
	$(APXS) -a -i -n $(NAME) $(TARGET) 

$(TARGET): $(OBJS) parser/parser.a
	$(CXX) $(INCLUDE) -shared -o $@ $^ $(LIBS) $(FLEX_LIB)

parser/parser.a: _always_check_
	$(MAKE) -C parser

_always_check_:

obj/:
	mkdir obj
obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $(INCLUDE) $<
clean:
	$(MAKE) -C parser $@
	rm -rf obj/ $(TARGET)
