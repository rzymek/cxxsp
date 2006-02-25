OBJS=obj/csplex.o obj/lexer.o obj/parser.o obj/generator.o obj/generator_manager.o
FLAGS=-I ../../cxxsp -I../../../common -I$(APACHE_HOME)/include

all: obj parser.a

parser.a: $(OBJS)
	ar cr $@ $^
obj:
	mkdir obj
obj/%.o: src/%.cpp 
	$(CXX) $(FLAGS) -o $@ -c $<
obj/csplex.o: src/csplex.cpp 
	$(CXX) -o $@ -I $(FLEX_INC) -c $<
src/csplex.cpp: src/csplex.l
	$(FLEX) -o$@ $<
back: clean
	(cd .. && zip ../../backups/parser-`date +%Y.%m.%d-%H.%M`.zip -r parser)
clean:
	rm -f parser.a obj/*.o
realclean: clean
	rm -f src/csplex.cpp
