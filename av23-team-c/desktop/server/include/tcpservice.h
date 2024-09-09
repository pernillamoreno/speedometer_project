#ifndef TCPCOM_H
#define TCPCOM_H

#include "comservice.h"
#include <thread>
#include <atomic>
#include <cctype>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TCPService : public COMService
{
    std::atomic<bool> running{true};
    std::thread serviceThread{&TCPService::run, this};
    int sockfd, connfd;
    sockaddr_in servaddr, cli;
    socklen_t len = sizeof(cli);

protected:
    /**
     * @brief Run the TCPService
     *
     */
    void run() override;

public:
    TCPService() = default;
    ~TCPService()
    {
        running = false;
        close(sockfd);
        serviceThread.join();
    }
};

#endif
