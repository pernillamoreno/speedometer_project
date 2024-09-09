#include "tcpservice.h"
#include <fcntl.h>

constexpr int BUFSIZE = Setting::Signal::BUFSIZE;
constexpr int PORT = Setting::TCP::PORT;

void TCPService::run()
{
    // Create socket and check
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd == -1)
    {
        std::cout << "Failed to create the socket..." << std::endl;
        std::exit(1);
    }

    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        std::cout << "Failed to set SO_REUSEADDR on the socket..." << std::endl;
        close(sockfd);
        std::exit(1);
    }

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if (bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        std::cout << "Failed to bind servaddr to the socket..." << std::endl;
        close(sockfd);
        std::exit(1);
    }

    if (listen(sockfd, 1) != 0) // only one connection
    {
        std::cout << "Failed to listen to the port..." << std::endl;
        close(sockfd);
        std::exit(1);
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        std::cout << "Failed to set socket to non-blocking mode..." << std::endl;
        close(sockfd);
        std::exit(1);
    }

    bzero(buffer, sizeof(buffer));
    bool waitingMessage{false};
    while (running)
    {
        if (!waitingMessage)
        {
            std::cout << "Waiting for a connection..." << std::endl;
            waitingMessage = true;
        }
        connfd = accept(sockfd, (sockaddr *)&cli, &len);
        if (connfd < 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL));
            continue;
        }
        std::cout << "Server accepted the client..." << std::endl;

        uint8_t tempBuffer[BUFSIZE]{0};
        while (running)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                memcpy(tempBuffer, buffer, BUFSIZE);
            }

            ssize_t bytes_written = write(connfd, tempBuffer, BUFSIZE);
            if (bytes_written != BUFSIZE)
            {
                std::cout << "Connection lost, attempting to reconnect..." << std::endl;
                close(connfd); // Close the current connection
                break;         // Exit the inner loop to accept a new connection
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
    }
}#include "tcpservice.h"
