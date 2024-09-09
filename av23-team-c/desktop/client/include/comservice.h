#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <mutex>
#include <climits>
#include <cmath>
#include <atomic>

class COMService
{
    /**
     * @brief Used to extract unsigned data from the buffer
     *
     * @param start starting bit
     * @param length length of the data in bits
     * @param data reference to the extracted data
     */
    void extract(uint8_t start, uint8_t length, uint16_t &data);
    /**
     * @brief Used to extract signed data from the buffer
     *
     * @param start starting bit
     * @param length length of the data in bits
     * @param data reference to the extracted data
     */
    void extract(uint8_t start, uint8_t length, int16_t &data);

public:
    /**
     * @brief Get the Speed from the buffer
     *
     * @return The speed in km/h
     */
    uint8_t getSpeed();

    /**
     * @brief Get the Battery Level from the buffer
     *
     * @return The battery level in percentage
     */
    uint8_t getBatteryLevel();

    /**
     * @brief Get the Temperature from the buffer
     *
     * @return The temperature in Celsius
     */
    int8_t getTemperature();

    /**
     * @brief Get the Left Indicator status from the buffer
     *
     * @return true If the left indicator is on
     * @return false If the left indicator is off
     */
    bool getLeft();

    /**
     * @brief Get the Right Indicator status from the buffer
     *
     * @return true If the right indicator is on
     * @return false If the right indicator is off
     */
    bool getRight();

    /**
     * @brief Get the status of the COMService
     *
     * @return true If the communication is working
     * @return false If the communication is not working
     */
    bool getStatus() const;
    Setting::Signal &signal = Setting::Signal::getInstance();

protected:
    uint8_t buffer[Setting::Signal::BUFSIZE]{0};
    std::mutex mtx;
    std::atomic<bool> status{false};

    /**
     * @brief Run the COMService
     *
     */
    virtual void run() = 0;
    virtual ~COMService() = default;
};

#endif // COMSERVICE_H
