#include <cstdint>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/i2c.h>

#define OLED_WIDTH          128
#define OLED_HEIGHT         32
#define OLED_BUFFER_SIZE    (OLED_WIDTH * OLED_HEIGHT / 8) // 512 byts
#define OLED_I2C_ADDR       0x3C

class SSD1306 
{
public:
    explicit SSD1306(const struct device *i2c, uint8_t addr = OLED_I2C_ADDR);

    bool init();
    void clear();
    bool update();

    void drawPixel(int16_t x, int16_t y, bool color);
    void drawChar(int16_t x, int16_t y, char c, bool color);
    void drawString(int16_t x, int16_t y, const char *str, bool color);

private:
    const struct device *i2c_;
    uint8_t addr_;
    uint8_t buffer_[OLED_BUFFER_SIZE];

    int writeCmd(uint8_t cmd);
    int writeData(const uint8_t *data, size_t len);
};