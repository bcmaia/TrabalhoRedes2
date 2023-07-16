// Client.cpp
#include <iostream>
#include "../module_1/socket.hpp"
#include "./utils.hpp"

#include <cstdio>
#include <mutex>
#include <thread>
#include <condition_variable>

std::mutex rePrintInputMtx;
std::mutex usingUserInput;
std::mutex printMtx;
std::mutex listenMtx;
std::condition_variable cv;

int serverDescriptor = 0;
bool listenning = false;
bool rePrintInputFlag = false;

std::string userInput = "";

void listenFunc()
{
	// Monitor logic
	std::unique_lock<std::mutex> lock(listenMtx);

	// std::cout << "listenning on!";

	while (listenning)
	{
		// std::cout << "listenning" << std::endl;
		try
		{
			fd_set readSet;
			FD_ZERO(&readSet);
			FD_SET(serverDescriptor, &readSet);

			struct timeval timeout;
			timeout.tv_sec = 1;	 // Set the timeout value in seconds
			timeout.tv_usec = 0; // Set the timeout value in microseconds

			// Use select to wait for data or timeout
			int selectResult = select(serverDescriptor + 1, &readSet, nullptr, nullptr, &timeout);

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
				if (FD_ISSET(serverDescriptor, &readSet))
				{
					std::string message = Socket::receive(serverDescriptor);

					std::unique_lock<std::mutex> lock(printMtx);
					std::cout << "\r";
					std::cout << "\033[K";
					std::cout << "\r";

					std::cout << message << std::endl;
					std::cout.flush();
				}
			}
		}
		catch (const runtime_error &e)
		{
			std::unique_lock<std::mutex> lock(printMtx);
			std::cerr << "Exception occurred: " << e.what() << std::endl;
			break;
		}
	}
}

int main()
{
	try
	{
		// Create a client socket
		Socket clientSocket(8080);

		bool active = true;
		bool connected = false;
		listenning = false;

		while (active)
		{
			{
				std::unique_lock<std::mutex> lock(usingUserInput);
				userInput = "";
			}
			char incommingChar;

			do
			{
				incommingChar = getchar();

				if ('\n' != incommingChar)
				{
					std::unique_lock<std::mutex> lock(usingUserInput);
					userInput += incommingChar;
				}

			} while ('\n' != incommingChar);

			

			
			std::string message;
			{
				std::unique_lock<std::mutex> lock(usingUserInput);
				message = userInput;
			}

			{
				std::unique_lock<std::mutex> lock(printMtx);
				std::cout << "> " << message << std::endl;
			}

			if (utils::hascmd(message, "/quit"))
			{
				{
					std::unique_lock<std::mutex> lock(printMtx);
					std::cout << "- Quitting..." << std::endl;
				}
				active = false;
				clientSocket.send("/quit");
			}
			else if (utils::hascmd(message, "/connect"))
			{
				{
					std::unique_lock<std::mutex> lock(printMtx);
					std::cout << "- Conecting... " << std::endl;
				}

				// Connect to the server
				clientSocket.connect();
				serverDescriptor = clientSocket.getDescriptor();

				connected = true;
				listenning = true;

				std::thread myListenner(listenFunc);
				myListenner.detach();

				{
					std::unique_lock<std::mutex> lock(printMtx);
					std::cout << "- Connection established... " << std::endl;
				}
			}

			// check for connection
			else if (!connected)
			{
				std::unique_lock<std::mutex> lock(printMtx);
				std::cout << "- [ERROR] You are not connected. " << std::endl;
				// Dont execute the other if clouses if not connected
			}

			// other commands
			else if (utils::hascmd(message, "/ping"))
			{
				clientSocket.send("/ping");
			}

			// process regular message
			else
			{
				clientSocket.send(message);
			}
		}

		listenning = false;

		{
			std::unique_lock<std::mutex> lock(printMtx);
			std::cout << "- All done! Bye ;) " << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
