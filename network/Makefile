## makefile to create example client server for testing
CC:= gcc
CFLAGS:= -Wall -g -c

%.o: %.c 
	$(CC) $(CFLAGS) *.c -lczmq -lzmq
client_test: client_test.o client.o
	$(CC) -Wall -o client_test.out client_test.o client.o -lczmq -lzmq -lpthread 
server_test: server_test.o server.o 
	$(CC) -Wall -o server_test.out server_test.o server.o -lczmq -lzmq -lpthread
all: client_test server_test
clean:
	rm -rf *.o *.out