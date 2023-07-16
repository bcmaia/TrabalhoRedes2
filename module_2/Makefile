all:
	make mk-client
	make mk-server

mk-client:
	g++ -pthread -o client client.cpp utils.cpp ../module_1/socket.cpp

mk-server:
	g++ -o server server.cpp utils.cpp ../module_1/socket.cpp

clean:
	rm server client