all:
	make mk-client
	make mk-server

mk-client:
	g++ -o client client.cpp socket.cpp

mk-server:
	g++ -o server server.cpp socket.cpp

clean:
	rm server client