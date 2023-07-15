// Client.cpp
#include <iostream>
#include "../module_1/socket.hpp"
#include "./utils.hpp"

#include <mutex>
#include <thread>
#include <condition_variable>

std::mutex printMtx;
std::mutex listenMtx;
std::condition_variable cv;
int serverDescriptor = 0;
bool listenning = false;
bool listenningActive = false;

void listenFunc() {
    std::unique_lock<std::mutex> lock(listenMtx);

    while (!listenning) {
        cv.wait(lock);
    }

	std::cout << "listenning on!";

    while (listenningActive) {
		std::cout << "listenning" << std::endl;
        try {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(serverDescriptor, &readSet);

            struct timeval timeout;
            timeout.tv_sec = 1; // Set the timeout value in seconds
            timeout.tv_usec = 0; // Set the timeout value in microseconds

            // Use select to wait for data or timeout
            int selectResult = select(serverDescriptor + 1, &readSet, nullptr, nullptr, &timeout);

            if (selectResult == -1) {
                // Handle select error
                // ...
				; // pass
            } else if (selectResult == 0) {
                // Handle timeout, no data received within the specified timeout
                // ...
				; // pass
            } else {
                // Data is available to read
                if (FD_ISSET(serverDescriptor, &readSet)) {
					std::unique_lock<std::mutex> lock(printMtx);
                    std::string message = Socket::receive(serverDescriptor);
                    std::cout << message;
                }
            }
        } catch (const runtime_error &e) {
			// TODO
            break;
        }
    }
}

// Function to put the thread to sleep
void timeToGoDeaf () {
	listenningActive = false;
    std::unique_lock<std::mutex> lock(listenMtx);
	listenning = false;
}

// Function to wake up the thread
void timeToListen () {
	listenningActive = true;
    std::unique_lock<std::mutex> lock(listenMtx);
    listenning = true;
    cv.notify_one();
}


int main()
{
	try
	{
		// Create a client socket
		Socket clientSocket(8080);

		

		// Send a message to the server
		bool active = true;
		bool connected = false;

		listenning = false;
		std::thread myListenner (listenFunc);

		while (active) {
			std::cout << "> ";
			std::string message;
			std::getline(std::cin, message);
			//std::cout << "\n";

			// Meta commands
			if (utils::hascmd(message, "/quit")) {
				std::unique_lock<std::mutex> lock(printMtx);
				std::cout << "- Quitting..." << std::endl;
				active = false;
				clientSocket.send("/quit");
			} else if (utils::hascmd(message, "/connect")) {
				std::unique_lock<std::mutex> lock(printMtx);
				connected = true;
				std::cout << "- Conecting... " << std::endl;

				// Connect to the server
				clientSocket.connect();

				serverDescriptor = clientSocket.getDescriptor();
				timeToListen();

				std::cout << "- Connection established... " << std::endl;
			} 

			// check for connection
			else if (!connected) {
				std::unique_lock<std::mutex> lock(printMtx);
				std::cout << "- [ERROR] You are not connected. " << std::endl;
				// Dont execute the other if clouses if not connected
			} 
			
			// other commands
			else if (utils::hascmd(message, "/ping")) {
				std::unique_lock<std::mutex> lock(printMtx);
				clientSocket.send("/ping");
				//std::string aaa = clientSocket.receive(clientSocket.getDescriptor());
				//std::cout << aaa << std::endl;
			} 
			
			// process regular message
			else {
				clientSocket.send(message);
				std::unique_lock<std::mutex> lock(printMtx);
				std::cout << "- Message sent to the server: '" 
					<< message << "'" << std::endl ;
			}

			
			std::cout << std::endl;
		}

		timeToGoDeaf ();
		myListenner.join();
		std::cout << "- All done! Bye ;) " << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	return 0;
}
