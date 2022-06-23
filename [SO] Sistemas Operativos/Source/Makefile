program: main.o functions.o
	 gcc -Wall -pthread main.o functions.o -o program

main.o: main.c header.h
	gcc -c main.c

functions.o: functions.c header.h
	gcc -c functions.c