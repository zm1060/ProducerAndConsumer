
all: main

main: main.o
	@echo "Checking.."
	gcc main.o -o main -lpthread

main.o: main.c
	@echo "Creating object.."
	gcc -c main.c

