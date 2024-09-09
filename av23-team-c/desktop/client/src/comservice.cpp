#include "comservice.h"
#include "comservice.h"

uint8_t COMService::getSpeed()
{
    uint16_t data;
    extract(signal["speed"].start, signal["speed"].length, data);
    return data;
}

uint8_t COMService::getBatteryLevel()
{
    uint16_t data;
    extract(signal["battery_level"].start, signal["battery_level"].length, data);
    return data;
}

int8_t COMService::getTemperature()
{
    int16_t data;
    extract(signal["temperature"].start, signal["temperature"].length, data);
    return data;
}

bool COMService::getLeft()
{
    uint16_t data;
    extract(signal["left_indicator"].start, signal["left_indicator"].length, data);
    return data;
}

bool COMService::getRight()
{
    uint16_t data;
    extract(signal["right_indicator"].start, signal["right_indicator"].length, data);
    return data;
}

bool COMService::getStatus() const
{
    return status;
}

void COMService::extract(uint8_t start, uint8_t length, uint16_t &data)
{
    std::lock_guard<std::mutex> lock(mtx);

    if (length > 0 && length <= (sizeof(int) * CHAR_BIT))
    {
        if (start + length <= sizeof(buffer) * CHAR_BIT)
        {
            data = 0;

            int pos = start % CHAR_BIT;
            int index = start / CHAR_BIT;

            for (size_t i = 0; i < length; i++)
            {
                uint8_t bit = (uint8_t)((buffer[index] >> pos) & 1);

                if (bit == 1)
                {
                    data |= (1 << i);
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

void COMService::extract(uint8_t start, uint8_t length, int16_t &data)
{
    extract(start, length, reinterpret_cast<uint16_t &>(data));

    if (data & (1u << (length - 1)))
    {
        data |= ((~0u) << length);
    }
}
