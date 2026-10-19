#pragma once

#include <cstdint>

#include <zephyr/drivers/adc.h>

class SoilMoistureSensor {
public:
    SoilMoistureSensor();

    int init();

    int readRaw(int16_t& raw);
    int readHumidityPercent(int& percent);
    void setCalibration(int16_t dry, int16_t wet);

private:
    static constexpr int SAMPLE_COUNT = 16;

    int16_t dry_raw_ = 4100;
    int16_t wet_raw_ = 1500;

    adc_sequence sequence_{};
    int16_t sample_buffer_[1]{};
};