#include <cstdint>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/i2c.h>

#define OLED_WIDTH          128
#define OLED_HEIGHT         32
#define OLED_BUFFER_SIZE    (OLED_WIDTH * OLED_HEIGHT / 8) // 512 byts
#define OLED_I2C_ADDR       0x3C

// ----- Команды SSD1306 (из даташита) -----
#define SSD1306_SET_MEM_MODE        0x20
#define SSD1306_SET_COL_ADDR        0x21
#define SSD1306_SET_PAGE_ADDR       0x22
#define SSD1306_SET_DISP_START_LINE 0x40
#define SSD1306_SET_CONTRAST        0x81
#define SSD1306_SET_CHARGE_PUMP     0x8D
#define SSD1306_SET_SEG_REMAP       0xA0
#define SSD1306_SET_ENTIRE_ON       0xA4
#define SSD1306_SET_NORM_DISP       0xA6
#define SSD1306_SET_MUX_RATIO       0xA8
#define SSD1306_SET_DISP_OFF        0xAE
#define SSD1306_SET_DISP_ON         0xAF
#define SSD1306_SET_COM_OUT_DIR     0xC0
#define SSD1306_SET_DISP_OFFSET     0xD3
#define SSD1306_SET_DISP_CLK_DIV    0xD5
#define SSD1306_SET_PRECHARGE       0xD9
#define SSD1306_SET_COM_PIN_CFG     0xDA
#define SSD1306_SET_VCOM_DET        0xDB

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