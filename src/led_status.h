#pragma once

#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>

class RgbLed
{
public:
    int init();

    int setRed();
    int setGreen();
    int setBlue();
    int turnOff();

    int updateMoisture(int soil_percent);

private:
    int setColor(uint8_t red, uint8_t green, uint8_t blue);
};