#include "comservice.h"#include "comservice.h"

void COMService::setSpeed(uint8_t speed)
{
    insert(signal["speed"].start, signal["speed"].length, speed);
}

void COMService::setBatteryLevel(uint8_t level)
{
    insert(signal["battery_level"].start, signal["battery_level"].length, level);
}

void COMService::setTemperature(int8_t temperature)
{
    insert(signal["temperature"].start, signal["temperature"].length, temperature);
}

void COMService::setLeft(bool left)
{
    insert(signal["left_indicator"].start, signal["left_indicator"].length, left);
}

void COMService::setRight(bool right)
{
    insert(signal["right_indicator"].start, signal["right_indicator"].length, right);
}

void COMService::insert(uint8_t start, uint8_t length, uint16_t data)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (length > 0 && length <= (sizeof(int) * CHAR_BIT))
    {
        if (start + length <= sizeof(buffer) * CHAR_BIT)
        {
            int pos = start % CHAR_BIT;
            int index = start / CHAR_BIT;

            for (size_t i = 0; i < length; i++)
            {
                uint8_t bit = (uint8_t)((data >> i) & 1);

                if (bit == 1)
                {
                    buffer[index] |= (1 << pos);
                }
                else
                {
                    buffer[index] &= ~(1 << pos);
                }

                pos++;
                if (pos == CHAR_BIT)
                {
                    pos = 0;
                    index++;
                }
            }
        }
    }
}
