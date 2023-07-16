// Client.cpp
#include <iostream>
#include "socket.hpp"

int main()
{
	try
	{
		// Create a client socket
		Socket clientSocket(8080);

		// Connect to the server
		clientSocket.connect();

		// Send a message to the server
		std::string message = "Hello, server!";
		clientSocket.send(message);

		std::cout << "Message sent to the server: " << message << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
