all: prog

prog: main.o vehicles.o managers.o auth.o menus.o utilities.o
	gcc -g -o prog main.o vehicles.o managers.o auth.o menus.o utilities.o

main.o: main.c
	gcc -g -o main.o main.c -c

vehicles.o: vehicles.c header.h
	gcc -g -o vehicles.o vehicles.c -c

managers.o: managers.c header.h
	gcc -g -o managers.o managers.c -c

auth.o: auth.c header.h
	gcc -g -o auth.o auth.c -c

menus.o: menus.c header.h
	gcc -g -o menus.o menus.c -c

utilities.o: utilities.c header.h
	gcc -g -o utilities.o utilities.c -c

clean:
	del /Q /S *.o *~ math

run:
	MinGW32-make clean
	MinGW32-make all
	./prog