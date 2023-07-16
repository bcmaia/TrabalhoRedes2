// Server.cpp
#include <iostream>
#include "socket.hpp"

int main()
{
	try
	{
		// Create a server socket
		Socket serverSocket(8080);

		// Bind the socket to a port
		serverSocket.bind();

		// Listen for incoming connections
		serverSocket.listen(5);

		std::cout << "Server is listening for connections..." << std::endl;

		// Accept a client connection
		int clientSocket = serverSocket.accept();

		// Receive and display the message from the client
		std::string receivedMessage = serverSocket.receive(clientSocket);
		std::cout << "Received message from the client: " << receivedMessage << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
