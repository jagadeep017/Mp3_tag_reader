OBJ=$(patsubst %.c,%.o,$(wildcard *.cpp))
a.out : $(OBJ)
	g++ -o $@ $^
clean :
	rm *.o *.out
