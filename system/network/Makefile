## makefile to create example client server for testing
CC:= gcc
CFLAGS:= -Wall -g -c

%.o: %.c 
	$(CC) $(CFLAGS) *.c -lzmq
conn_test: conn_test.o server.o client.o
	$(CC) -Wall -o conn_test *.o -lzmq
clean:
	rm -rf *.o