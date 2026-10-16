#include "scanner.h"
#include "oled.h"
#include "soil_sensor.h"
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define WIFI_STACK_SIZE 4096
#define OLED_STACK_SIZE 2048

#define WIFI_PRIORITY 5
#define OLED_PRIORITY 5

K_THREAD_STACK_DEFINE(wifi_stack, WIFI_STACK_SIZE);
K_THREAD_STACK_DEFINE(oled_stack, OLED_STACK_SIZE);
K_THREAD_STACK_DEFINE(soil_stack, OLED_STACK_SIZE);

static struct k_thread wifi_thread_data;
static struct k_thread oled_thread_data;
static struct k_thread soil_thread_data;

static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
static SSD1306 oled(i2c, OLED_I2C_ADDR);
static WifiScanner scanner;
static SoilMoistureSensor sensor;

static void wifi_thread_entry(void *p1, void *p2, void *p3)
{
    auto *scanner = static_cast<WifiScanner *>(p1);

    printk("Wi-Fi thread started\n");

    int ret = scanner->start();
    printk("Wi-Fi scanner stopped: %d\n", ret);
}

static void oled_thread_entry(void *p1, void *p2, void *p3)
{
    auto *display = static_cast<SSD1306 *>(p1);
    auto *scanner = static_cast<WifiScanner *>(p2);
    auto *sensor = static_cast<SoilMoistureSensor *>(p3);
    char text[32];
    printk("OLED thread started\n");

    display->clear();
    display->drawString(0, 0, "ESP32-S3", true);
    display->drawString(0, 10, "Wi-Fi scanner", true);
    display->update();

    while (true)
    {
        int count = scanner->getNetworkCount();
        int soil_percent;
        sensor->readHumidityPercent(soil_percent);

        display->clear();
        display->drawString(0, 0, "Wi-Fi scan result:", true);

        snprintk(text, sizeof(text), "Found %d networks", count);
        display->drawString(0, 12, text, true);

        snprintk(text, sizeof(text), "Value of humidity %d", soil_percent);
        display->drawString(0, 24, text, true);

        display->update();

        k_sleep(K_SECONDS(1));
    }
}

static void soil_thread_entry(void *p1, void *p2, void *p3)
{
    auto *sensor = static_cast<SoilMoistureSensor *>(p1);

    while (true) {
        int16_t raw = 0;
        const int raw_ret = sensor->readRaw(raw);

        if (raw_ret < 0) 
        {
            printk("ERROR: ADC read failed: %d\n", raw_ret);
            k_sleep(K_SECONDS(2));
            continue;
        }

        int humidity = 0;
        const int humidity_ret = sensor->readHumidityPercent(humidity);

        if (humidity_ret < 0) {
            printk("ERROR: Humidity calculation failed: %d\n", humidity_ret);
            k_sleep(K_SECONDS(2));
            continue;
        }

        printk("ADC raw: %d | Soil moisture: %d%%\n", raw, humidity);

        k_sleep(K_SECONDS(2));
    }

}

int main()
{
    printk("Application startingssssss\n");

    // if (!device_is_ready(i2c))
    // {
    //     printk("ERROR: I2C device is not ready\n");
    //     return 0;
    // }

    // if (!oled.init())
    // {
    //     printk("ERROR: OLED initialization failed\n");
    //     return 0;
    // }

    // if (!sensor.init())
    // {
    //     printk("ERROR: ADC initialization failed\n");
    //     return 0;
    // }

    // // Wi-Fi stack.
    // k_thread_create( &wifi_thread_data, wifi_stack, K_THREAD_STACK_SIZEOF(wifi_stack), 
    //                 wifi_thread_entry, &scanner, nullptr, nullptr, WIFI_PRIORITY, 0, K_NO_WAIT);

    // // OLED stack.
    // k_thread_create(&oled_thread_data, oled_stack, K_THREAD_STACK_SIZEOF(oled_stack),
    //                 oled_thread_entry, &oled, &scanner, &sensor, OLED_PRIORITY, 0, K_NO_WAIT);

    // // Soil sensor stack
    // k_thread_create(&soil_thread_data, soil_stack, K_THREAD_STACK_SIZEOF(oled_stack),
    //                 soil_thread_entry, &sensor, nullptr, nullptr, OLED_PRIORITY, 0, K_NO_WAIT);

    while (true)
    {
        printk("Before message\n");
        printk("All threads created\n");
        printk("After message\n");
        k_sleep(K_SECONDS(10));
    }
    

    return 0;
}