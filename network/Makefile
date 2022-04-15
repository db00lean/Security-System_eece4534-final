## makefile to create example client server for testing
CFLAGS:= -Wall -g -c -O

%.o: %.c 
	$(CC) $(CFLAGS) *.c -lczmq -lzmq
client_test: client_test.o client.o packet.o
	$(CC) -Wall -o client_test.out client_test.o client.o packet.o -lczmq -lzmq -lpthread 
server_test: server_test.o server.o packet.o
	$(CC) -Wall -o server_test.out server_test.o server.o packet.o -lczmq -lzmq -lpthread
all: client_test server_test
cplus: client.c
	${CXX} -c client.c
clean:
	rm -rf *.o *.out