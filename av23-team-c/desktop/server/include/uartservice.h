
#ifndef UARTCOM_H
#define UARTCOM_H

#include "comservice.h"
#include <QSerialPort>
#include <QThread>
#include <atomic>

class UARTService : public COMService, public QThread
{
    std::atomic<bool> runningUART{true};

protected:
    /**
     * @brief Run the UARTService
     *
     */
    void run() override;

public:
    UARTService()
    {
        start();
    }
    ~UARTService()
    {
        runningUART = false;

        wait();
    }
};
