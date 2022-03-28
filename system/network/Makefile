## makefile to create example client server for testing
client: client.c 
	gcc -c client.c
server: server.c
	gcc -c server.c
conn_test: conn_test.c server.o client.o
	gcc -o conn_test server.c client.c
clean:
	rm -rf *.o