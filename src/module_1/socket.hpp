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

using namespace std;

class Socket
{

public:
	Socket(unsigned short port);
	~Socket();
	void bind (void);

	static int send(int fd, string msg, int attempt);
    static string receive(int fd);
    void listen(int maxConnections);
    int accept();
    void connect();

	int getDescriptor() const {
        return descriptor;
    }

private:
	int32_t descriptor; // socket file descriptor
	struct sockaddr_in address;

};