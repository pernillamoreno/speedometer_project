
#include "uartservice.h"
#include <QDebug>
#include <chrono>
#include <QSerialPort>

void UARTService::run()
{
    QSerialPort UART;
    UART.setPortName(Setting::UART::SERVER_PORT);
    UART.setBaudRate(Setting::UART::BAUDRATE);
    UART.setDataBits(QSerialPort::Data8);
    UART.setParity(QSerialPort::NoParity);
    UART.setStopBits(QSerialPort::OneStop);
    UART.setFlowControl(QSerialPort::NoFlowControl);

    bool messagePrinted;

    while (runningUART)
    {
        // Try to open the serial port once
        if (UART.open(QIODevice::WriteOnly))
        {
            messagePrinted = false;
            qDebug() << "Serial port opened successfully!";

            while (runningUART && UART.isWritable())
            {
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{mtx};
                    std::memcpy(temp, buffer, sizeof(temp));
                    (void)UART.clear();
                }

                if (sizeof(temp) == UART.write(reinterpret_cast<char *>(temp), sizeof(temp)))
                {
                    if (!UART.flush())
                    {
                        qDebug() << "Not all data was written to the serial port.";
                        break;
                    }
                }
                else
                {
                    qDebug() << "Not all data was written to the serial port.";
                    break;
                }
                msleep(Setting::INTERVAL);
            }
        }
        else
        {
            if (!messagePrinted)
            {
                qDebug() << "Failed to open serial port:" << UART.errorString();
                qDebug() << "Retrying to open the serial port...";
                messagePrinted = true;
            }
        }
        // Close the serial port
        if (UART.isOpen())
        {
            UART.close();
        }
    }
}
