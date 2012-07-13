ARGS= -std=c99 -pthread

all: socket.o rs232.o main.o
	gcc socket.o rs232.o main.o -o thomas ${ARGS}
	
main.o: main.c
	gcc -c main.c ${ARGS}
	
socket.o: socket.c
	gcc -c socket.c ${ARGS}
	
rs232.o: rs232.c
	gcc -c rs232.c ${ARGS}
	
clean:
	rm -rf ./*.o thomas