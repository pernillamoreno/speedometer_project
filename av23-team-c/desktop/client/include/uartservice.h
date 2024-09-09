#ifndef UARTSERVICE_H
#define UARTSERVICE_H

#include <QThread>
#include <QSerialPort>
#include <QByteArray>
#include "comservice.h"
#include "setting.h"

class UARTService : public COMService, public QThread
{
public:
    UARTService();
    ~UARTService();

protected:
    /**
     * @brief Run the UARTService
     *
     */
    void run() override;

private:
    bool isRunning{true};
};

#endif
