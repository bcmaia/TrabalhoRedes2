#pragma once

#include <cstring>      // for memset
#include <stdexcept>    // for std::runtime_error
#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE (4096)
#define TIME_OUT (600)

using namespace std;

class Socket
{

public:
	Socket(uint16_t port);
	~Socket();
	void bind (void);

	int send(string msg, int attempt = 1);
    static string receive(int32_t sourceDescriptor);
    void listen(int maxConnections);
    int accept();
    void connect();

	static int sendthis (int32_t descriptor, string msg, int attempt = 3);

	int getDescriptor() const {
        return descriptor;
    }

private:
	int32_t descriptor; // socket file descriptor
	struct sockaddr_in address;;
};