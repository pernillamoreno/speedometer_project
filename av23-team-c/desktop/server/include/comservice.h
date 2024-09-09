#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <mutex>
#include <climits>
#include <atomic>

class COMService
{
public:
    /**
     * @brief Insert the speed into the buffer
     *
     * @param speed The speed in km/h
     */
    void setSpeed(uint8_t speed);

    /**
     * @brief Insert the battery level into the buffer
     *
     * @param level The battery level in percentage
     */
    void setBatteryLevel(uint8_t level);

    /**
     * @brief Insert the temperature into the buffer
     *
     * @param temperature The temperature in Celsius
     */
    void setTemperature(int8_t temperature);

    /**
     * @brief Insert the left indicator status into the buffer
     *
     * @param left true If the left indicator is on
     */
    void setLeft(bool left);

    /**
     * @brief Insert the right indicator status into the buffer
     *
     * @param right true If the right indicator is on
     */
    void setRight(bool right);

    /**
     * @brief Insert the warning indicator status into the buffer
     *
     * @param warning true If the warning indicator is on
     */
    void setWarning(bool warning);
    Setting::Signal &signal = Setting::Signal::getInstance();

protected:
    uint8_t buffer[Setting::Signal::BUFSIZE]{0};
    std::mutex mtx;
    std::atomic<bool> status{false};

    /**
     * @brief Insert data into the buffer
     *
     * @param start starting bit
     * @param length length of the data in bits
     * @param data data to be inserted
     */
    void insert(uint8_t start, uint8_t length, uint16_t data);

    /**
     * @brief Run the COMService
     *
     */
    virtual void run() = 0;
};

#endif // COMSERVICE_H
