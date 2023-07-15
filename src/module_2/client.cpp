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

		

		// Send a message to the server
		bool active = true;
		bool connected = false;

		while (active) {
			std::cout << "> ";
			std::string message;
			std::getline(std::cin, message);
			//std::cout << "\n";

			// Meta commands
			if (utils::hascmd(message, "/quit")) {
				std::cout << "- Quitting..." << std::endl;
				active = false;
				clientSocket.send("/quit");
				std::cout << "- All done! Bye ;) " << std::endl;
			} else if (utils::hascmd(message, "/connect")) {
				connected = true;
				std::cout << "- Conecting... " << std::endl;
				// Connect to the server
				clientSocket.connect();
				std::cout << "- Connection established... " << std::endl;
			} 

			// check for connection
			else if (!connected) {
				std::cout << "- [ERROR] You are not connected. " << std::endl;
				// Dont execute the other if clouses if not connected
			} 
			
			// other commands
			else if (utils::hascmd(message, "/ping")) {
				clientSocket.send("/ping");
				std::string aaa = clientSocket.receive(clientSocket.getDescriptor());
				std::cout << aaa << std::endl;
			} 
			
			// process regular message
			else {
				clientSocket.send(message);
				std::cout << "- Message sent to the server: '" 
					<< message << "'" << std::endl ;
			}

			
			std::cout << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
