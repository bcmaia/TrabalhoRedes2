#include "./socket.hpp"

Socket::Socket(uint16_t port)
{
    // Initializing the address structure to 0 to avoid errors.
    memset(&address, 0, sizeof(address));

    // Create a TCP socket
    this->descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed
    if (-1 == this->descriptor) {
        throw std::runtime_error("Failed to create socket");
    }

    // Enable socket address reuse option
    int options = 1; // 1 means reuse option is raised
    if (-1 == setsockopt(
        this->descriptor,
        SOL_SOCKET,
        SO_REUSEADDR,
        &options,
        sizeof(options)
    )) {
        throw std::runtime_error("Failed to set up socket options");
    }

    // Set up the socket address
    this->address.sin_family = AF_INET;         // IPv4 address family
    this->address.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface
    this->address.sin_port = htons(port);       // Set the port number
    // obs: htons will make sure the order of bytes of port is correct (endianness)
}

Socket::~Socket()
{
    // Close the socket descriptor
    close(descriptor);
}

void Socket::bind(void)
{
    if (0 > ::bind(
        this->descriptor,
        (struct sockaddr *)&address,
        sizeof(address)
    )) {
        throw std::runtime_error("Failed to bind socket");
    }
    // The socket is now bound to the specified port and address
}

int Socket::send(int fd, string msg, int attempt)
{
    // Send the message to the specified file descriptor
    ssize_t bytesSent = write(fd, msg.c_str(), msg.length());

    if (bytesSent < 0)
    {
        throw std::runtime_error("Failed to send message");
    }

    return static_cast<int>(bytesSent);
}

string Socket::receive(int fd)
{
    char buffer[MAX_MSG_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // Receive the message from the specified file descriptor
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        throw std::runtime_error("Failed to receive message");
    }

    return string(buffer);
}

void Socket::listen(int maxConnections)
{
    // Listen for incoming connections on the socket
    if (0 > ::listen(descriptor, maxConnections))
    {
        throw std::runtime_error("Failed to listen on socket");
    }
    // The socket is now in the listening state and can accept connections
}

int Socket::accept()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Accept an incoming connection and get the client's socket descriptor
    int clientSocket = ::accept(descriptor, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (clientSocket < 0) {
        throw std::runtime_error("Failed to accept connection");
    }

    return clientSocket;
}

void Socket::connect()
{
    // Connect the socket to the specified address
    if (0 > ::connect(descriptor, (struct sockaddr *)&address, sizeof(address))) {
        throw std::runtime_error("Failed to connect to server");
    }
    // The socket is now connected to the remote server
}
