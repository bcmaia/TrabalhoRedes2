#include "./socket.hpp"

Socket::Socket(uint16_t port)
{
    // Initializing the address structure to 0 to avoid errors.
    memset(&address, 0, sizeof(address));

    // Create a TCP socket
    this->descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed
    if (-1 == this->descriptor)
    {
        throw std::runtime_error("Failed to create socket");
    }

    // Enable socket address reuse option
    int options = 1; // 1 means reuse option is raised
    if (-1 == setsockopt(
                  this->descriptor,
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  &options,
                  sizeof(options)))
    {
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
                sizeof(address)))
    {
        throw std::runtime_error("Failed to bind socket");
    }
    // The socket is now bound to the specified port and address
}

int Socket::send(string msg, int attempt)
{
    // Split the message into chunks of maximum size (4096)
    size_t msgLength = msg.length();
    size_t chunkSize = MAX_MSG_SIZE - 1; // Leave space for null terminator
    size_t numChunks = (msgLength + chunkSize - 1) / chunkSize;

    // Iterato over each message chunk
    for (size_t i = 0; i < numChunks; i++)
    {
        // Calculate the starting and ending positions for the current chunk
        size_t start = i * chunkSize;
        size_t end = start + chunkSize;
        if (end > msgLength)
        {
            end = msgLength;
        }

        // Extract the current chunk from the message
        string chunk = msg.substr(start, end - start);

        // Send the current chunk to the specified file descriptor
        ssize_t chuckLen = static_cast<ssize_t>(chunk.length());

        // Attempts to send.
        for (int j = attempt; 0 < j; j--)
        {
            ssize_t bytesSent = write(this->descriptor, chunk.c_str(), chuckLen);
            if (chuckLen == bytesSent)
                break; // sucess!
            else if (0 < j)
                continue; // try again
            else
                throw std::runtime_error("Failed to send message"); // defeat
        }
    }

    return static_cast<int>(msgLength);
}

int Socket::sendthis (int32_t descriptor, string msg, int attempt)
{
    // Split the message into chunks of maximum size (4096)
    size_t msgLength = msg.length();
    size_t chunkSize = MAX_MSG_SIZE - 1; // Leave space for null terminator
    size_t numChunks = (msgLength + chunkSize - 1) / chunkSize;

    // Iterato over each message chunk
    for (size_t i = 0; i < numChunks; i++)
    {
        // Calculate the starting and ending positions for the current chunk
        size_t start = i * chunkSize;
        size_t end = start + chunkSize;
        if (end > msgLength)
        {
            end = msgLength;
        }

        // Extract the current chunk from the message
        string chunk = msg.substr(start, end - start);

        // Send the current chunk to the specified file descriptor
        ssize_t chuckLen = static_cast<ssize_t>(chunk.length());

        // Attempts to send.
        for (int j = attempt; 0 < j; j--)
        {
            ssize_t bytesSent = write(descriptor, chunk.c_str(), chuckLen);
            if (chuckLen == bytesSent)
                break; // sucess!
            else if (0 < j)
                continue; // try again
            else
                throw std::runtime_error("Failed to send message"); // defeat
        }
    }

    return static_cast<int>(msgLength);
}

std::string Socket::receive(int32_t sourceDescriptor)
{
    char buffer[MAX_MSG_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // Receive the message from the specified file descriptor
    ssize_t bytesRead = recv(sourceDescriptor, buffer, sizeof(buffer) - 1, 0);

    if (0 > bytesRead)
    {
        // Error occurred while receiving data
        throw std::runtime_error("Failed to receive message");
    }
    else if (0 == bytesRead)
    {
        // Connection closed by the client
        throw std::runtime_error("Connection closed by the client");
    }

    return std::string(buffer);
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

int32_t Socket::accept()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Accept an incoming connection and get the client's socket descriptor
    int clientSocket = ::accept(descriptor, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (0 > clientSocket)
    {
        throw std::runtime_error("Failed to accept connection");
    }

    // Set receive timeout to 5 seconds for the client socket
    struct timeval timeout;
    timeout.tv_sec = TIME_OUT;
    timeout.tv_usec = 0;
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    return clientSocket;
}

void Socket::connect()
{
    // Connect the socket to the specified address
    if (0 > ::connect(descriptor, (struct sockaddr *)&address, sizeof(address)))
    {
        throw std::runtime_error("Failed to connect to server");
    }
    // The socket is now connected to the remote server
}
