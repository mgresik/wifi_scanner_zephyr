#include "led_status.h"

#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#define LED_STRIP_NODE DT_ALIAS(led_strip)

static const struct device *led_strip = DEVICE_DT_GET(LED_STRIP_NODE);

constexpr int MOISTURE_MIN = 30;
constexpr int MOISTURE_MAX = 70;

int RgbLed::init()
{
    printk("RGB LED device name: %s\n", led_strip->name);

    if (!device_is_ready(led_strip))
    {
        printk("RGB LED device is NOT ready\n");
        return -ENODEV;
    }

    printk("RGB LED device is ready\n");

    return turnOff();

}

int RgbLed::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    struct led_rgb color = {
        .r = red,
        .g = green,
        .b = blue
    };

    int ret = led_strip_update_rgb(led_strip, &color, 1);

    if (ret < 0)
    {
        // LOG_ERR("Failed to update RGB LED: %d", ret);
    }

    return ret;
}

int RgbLed::setRed()
{
    return setColor(255, 0, 0);
}

int RgbLed::setGreen()
{
    return setColor(0, 255, 0);
}

int RgbLed::setBlue()
{
    return setColor(0, 0, 255);
}

int RgbLed::turnOff()
{
    return setColor(0, 0, 0);
}

int RgbLed::updateMoisture(int soil_percent)
{
    if (soil_percent <= MOISTURE_MIN)
    {
        return setRed();
    }

    if (soil_percent >= MOISTURE_MAX)
    {
        return setBlue();
    }

    return setGreen();
}