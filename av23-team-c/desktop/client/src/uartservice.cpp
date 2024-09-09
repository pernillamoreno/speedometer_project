#include "uartservice.h"
#include <QDebug>

// Constructor: Initializes an instance of the UARTService class
UARTService::UARTService() : isRunning(true)
{
    start();
}

// Destructor: Cleans up resources when the UARTService object is destroyed
UARTService::~UARTService()
{
    isRunning = false; // Set isRunning to false to stop the run loop
    wait();            // Wait for the thread to finish execution
}

// Runs in a separate thread to continuously read data from the serial port
void UARTService::run()
{
    QSerialPort serialPort;
    serialPort.setPortName(Setting::UART::CLIENT_PORT);
    serialPort.setBaudRate(Setting::UART::BAUDRATE);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    while (isRunning)
    {
        if (serialPort.open(QIODevice::ReadOnly))
        {
            while (isRunning && serialPort.isReadable())
            {
                uint8_t temp[sizeof(buffer)]{0};
                serialPort.clear();

                if (serialPort.waitForReadyRead(Setting::INTERVAL * 2))
                {
                    if (sizeof(temp) == serialPort.read(reinterpret_cast<char *>(temp), sizeof(temp)))
                    {
                        status = true;
                        std::scoped_lock<std::mutex> lock{mtx};
                        std::memcpy(buffer, temp, sizeof(buffer));
                    }
                    else
                    {
                        qDebug() << "Failed to read data from the serial port.";
                        status = false;
                        break;
                    }
                }
                else
                {
                    status = false;
                    break;
                }
            }
        }
        else
        {
            status = false;
        }
        if (serialPort.isOpen())
        {
            serialPort.close();
        }
    }
}
