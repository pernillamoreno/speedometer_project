#include <CAN.h>
#include <Arduino.h>
#include <CAN_config.h>
#include "setting.h"

CAN_device_t CAN_cfg;

void setup()
{
    Serial.begin(Setting::UART::BAUDRATE);
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_35;
    CAN_cfg.speed = CAN_SPEED_500KBPS;

    CAN_init();
}

void loop()
{
    CAN_frame_t frame{0};
    frame.FIR.B.DLC = Setting::Signal::BUFSIZE;

    if (Serial.readBytes(frame.data.u8, Setting::Signal::BUFSIZE) == Setting::Signal::BUFSIZE)
    {
        CAN_write_frame(&frame);
    }
}