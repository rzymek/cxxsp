OBJS=src/libmain.o src/libyywrap.o
TARGET=libfl.a
$(TARGET): $(OBJS)
	ar cr $@ $^
%.o: %.cpp 
	$(CXX) $(FLAGS) -o $@ -c $<
clean:
	rm -f $(OBJS) $(TARGET)
