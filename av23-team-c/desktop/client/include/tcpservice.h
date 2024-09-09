#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <thread>
#include <atomic>
#include <sys/socket.h>

class TCPService : public COMService
{
    std::atomic<bool> running{true};
    std::thread serviceThread{&TCPService::run, this};

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
        serviceThread.join();
    }
};

#endif // TCPSERVICE_H
