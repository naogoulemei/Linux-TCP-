all:
	gcc tcp_server.c -o tcp_server
	gcc tcp_client.c -o tcp_client
clean:
	rm -rf tcp_server tcp_client

