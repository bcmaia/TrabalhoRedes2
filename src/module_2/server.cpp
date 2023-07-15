#include <iostream>
#include "../module_1/socket.hpp"
#include "./utils.hpp"

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

		for ever {
			// Accept a client connection
			int clientSocket = serverSocket.accept();

			// Receive and display messages from the client until the connection is closed or a timeout occurs
			while (true) {
				try {
					std::string receivedMessage = serverSocket.receive(clientSocket);
					std::cout << "Received message from the client: " << receivedMessage << std::endl;
				}
				catch (const std::runtime_error &e) {
					std::cerr << "Error: " << e.what() << std::endl;
					break; // Exit the inner loop if an error occurs during receiving
				}
			}

			std::cout << "Client connection closed or timed out." << std::endl;

			// Close the client socket
			close(clientSocket);
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
