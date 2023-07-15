// Client.cpp
#include <iostream>
#include "../module_1/socket.hpp"
#include "./utils.hpp"

int main()
{
	try
	{
		// Create a client socket
		Socket clientSocket(8080);

		// Connect to the server
		clientSocket.connect();

		// Send a message to the server
		bool quitFlag = false;
		for (;;) {
			std::cout << "> ";
			std::string message;
			std::getline(std::cin, message);
			std::cout << "\n";

			if (utils::hascmd(message, "/quit")) {
				quitFlag = true;
				std::cout << "- Quitting...";
			}

			clientSocket.send(message);
			std::cout << "Message sent to the server: " << message << std::endl;

			if (quitFlag) break;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
