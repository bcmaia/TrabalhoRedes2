#include <iostream>
#include "../module_1/socket.hpp"
#include "./utils.hpp"

#include <mutex>
#include <thread>
#include <list>
#include <condition_variable>

#define MAX_CONNECTIONS (20)

std::mutex printMtx;

std::list<int32_t> clients;
std::list<std::thread> threads;

std::mutex sendMsgMtx;
std::condition_variable sendMsgCv;
bool sending = false;

std::mutex clientListMtx;
std::condition_variable clientListCv;

#define ALL (0)

void removeClient(int32_t client)
{
	std::unique_lock<std::mutex> lock(clientListMtx);
	clients.remove(client);
}

void addClient(int32_t client)
{
	std::unique_lock<std::mutex> lock(clientListMtx);
	clients.push_back(client);
}

void sendMenssage(int32_t sender, std::string msg, bool toAll = false)
{
	std::unique_lock<std::mutex> lock(sendMsgMtx);

	{
		std::unique_lock<std::mutex> lock(printMtx);
		std::cerr << "- Sending msg: " << msg << std::endl;
	}

	if (toAll)
		for (int32_t client : clients)
		{
			{
				std::unique_lock<std::mutex> lock(printMtx);
				std::cerr << "- sending to client: " << client << std::endl;
			}
			Socket::sendthis(client, msg);
		}
	else
		for (int32_t client : clients)
		{
			if (client != sender)
			{
				{
					std::unique_lock<std::mutex> lock(printMtx);
					std::cerr << "- sending to client: " << client << std::endl;
				}
				Socket::sendthis(client, msg);
			}
		}

	{
		std::unique_lock<std::mutex> lock(printMtx);
		std::cerr << "- Sent msg: " << msg << std::endl;
	}
}

void listenClientFunc(int32_t clientDescriptor, int index)
{
	// Receive and display messages from the client until the connection is closed or a timeout occurs
	bool active = true;
	while (active)
	{
		try
		{
			bool recieved = false;
			std::string receivedMessage = "";
			// std::string receivedMessage = Socket::receive(clientDescriptor);

			fd_set readSet;
			FD_ZERO(&readSet);
			FD_SET(clientDescriptor, &readSet);

			struct timeval timeout;
			timeout.tv_sec = 1;	 // Set the timeout value in seconds
			timeout.tv_usec = 0; // Set the timeout value in microseconds

			// Use select to wait for data or timeout
			int selectResult = select(clientDescriptor + 1, &readSet, nullptr, nullptr, &timeout);

			if (selectResult == -1)
			{
				// Handle select error
				// ...
				continue; // pass
			}
			else if (selectResult == 0)
			{
				// Handle timeout, no data received within the specified timeout
				// ...
				continue; // pass
			}
			else
			{
				// Data is available to read
				if (FD_ISSET(clientDescriptor, &readSet))
				{
					receivedMessage = Socket::receive(clientDescriptor);
					recieved = true;
				}
			}

			if (!recieved)
				continue;

			if (utils::hascmd(receivedMessage, "/quit"))
			{
				active = false;
			}
			else if (utils::hascmd(receivedMessage, "/connect"))
			{
				; // pass
			}
			else if (utils::hascmd(receivedMessage, "/ping"))
			{
				// Send a "pong" response back to the client
				sendMenssage(ALL, "- pong", true);
			}
			else
			{
				sendMenssage(clientDescriptor, std::to_string(index) + ": " + receivedMessage);
			}
		}
		catch (const std::runtime_error &e)
		{
			if (active)
			{
				std::unique_lock<std::mutex> lock(printMtx);
				std::cerr << "- [Error] " << e.what() << std::endl;
			}
			break; // Exit the inner loop if an error occurs during receiving
		}
	}

	std::unique_lock<std::mutex> lock(printMtx);
	std::cout << "- Client connection closed or timed out." << std::endl;

	// Close the client socket
	removeClient(clientDescriptor);
	close(clientDescriptor);
}

int main()
{
	try
	{
		// Create a server socket
		Socket serverSocket(8080);

		// Bind the socket to a port
		serverSocket.bind();

		// Listen for incoming connections
		serverSocket.listen(MAX_CONNECTIONS);

		std::cout << "Server is listening for connections..." << std::endl;

		for (int index;; index++)
		{
			// Accept a client connection
			int clientSocket = serverSocket.accept();
			addClient(clientSocket);

			std::unique_lock<std::mutex> lock(printMtx);
			std::cerr << "- Connected to " << clientSocket << std::endl;

			std::thread myThread([clientSocket, index]()
								 { listenClientFunc(clientSocket, index); });

			myThread.detach();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
