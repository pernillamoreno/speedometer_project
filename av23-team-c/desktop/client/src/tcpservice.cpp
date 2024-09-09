#include "tcpservice.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <chrono>
#include <thread>

constexpr int BUFSIZE = Setting::Signal::BUFSIZE;
constexpr int PORT = Setting::TCP::PORT;
const char *SERVER = Setting::TCP::SERVER;

void TCPService::run()
{
    int sockfd;
    sockaddr_in servaddr{};

    while (running)
    {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER);

        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            std::cout << "Socket creation failed" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue; // Attempt to recreate the socket
        }

        bool MessagePrinted{false};
        // Connect to the server
        while (running && connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            if (!MessagePrinted)
            {
                std::cout << "Connection to the server failed, attempting to reconnect..." << std::endl;
                MessagePrinted = true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL));

            close(sockfd);
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0)
            {
                std::cout << "Socket recreation failed" << std::endl;
                continue; // Attempt to recreate the socket again
            }
        }

        if (running)
        {
            std::cout << "Connected to the server." << std::endl;
        }

        bzero(buffer, sizeof(buffer));
        uint8_t tempBuffer[BUFSIZE]{0};
        while (running)
        {
            ssize_t bytesReceived = read(sockfd, tempBuffer, BUFSIZE);
            if (bytesReceived <= 0)
            {
                std::cout << "Connection lost, attempting to reconnect..." << std::endl;
                status = false;
                break;
            }
            else
            {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    memcpy(buffer, tempBuffer, BUFSIZE);
                }
                status = true;
            }
        }
    }
    close(sockfd);
}#include "tcpservice.h"
