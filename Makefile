program.o: program.c program.h
	gcc -c program.c -o program.o

program: program.o
	gcc program.o -o program

clean:
	-rm -f program.o
	-rm -f program

run: program
	./program
