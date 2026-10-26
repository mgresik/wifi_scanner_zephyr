#include "soil_sensor.h"

#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <cerrno>
#include <cstdint>

static const struct adc_dt_spec ADC_SPEC = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

SoilMoistureSensor::SoilMoistureSensor(){}

int SoilMoistureSensor::init()
{
    if (!adc_is_ready_dt(&ADC_SPEC)) {
        return -ENODEV;
    }

    const int ret = adc_channel_setup_dt(&ADC_SPEC);
    if (ret < 0) 
    {
        return ret;
    }

    sequence = {};

    const int seq_ret = adc_sequence_init_dt(&ADC_SPEC, &sequence);
    if (seq_ret < 0) 
    {
        return seq_ret;
    }

    sequence.buffer = sample_buffer;
    sequence.buffer_size = sizeof(sample_buffer);
    sequence.resolution = 12;
    sequence.oversampling = 0;

    return 0;
}

int SoilMoistureSensor::readRaw(int16_t& raw)
{
    int32_t sum = 0;

    for (int i = 0; i < SAMPLE_COUNT; ++i) 
    {    
        sample_buffer[0] = 0;

        sequence.buffer = sample_buffer;
        sequence.buffer_size = sizeof(sample_buffer);
        sequence.resolution = 12;
        sequence.channels = BIT(ADC_SPEC.channel_id);
        
        const int ret = adc_read(ADC_SPEC.dev, &sequence);
        if (ret < 0) 
        {
            return ret;
        }

        sum += sample_buffer[0];
        k_sleep(K_MSEC(2));
    }
    
    raw = static_cast<int16_t>(sum / SAMPLE_COUNT);
    
    return 0;
}

int SoilMoistureSensor::readHumidityPercent(int& percent)
{
    int16_t raw = 0;

    const int ret = readRaw(raw);
    if (ret < 0) 
    {
        return ret;
    }

    const int32_t range = static_cast<int32_t>(wet_raw_) - dry_raw_;
    if (range == 0) 
    {
        return -EINVAL;
    }

    int32_t value = (static_cast<int32_t>(raw) - dry_raw_) * 100 / range;

    if (value < 0) 
    {
        value = 0;
    }

    if (value > 100) 
    {
        value = 100;
    }

    percent = static_cast<int>(value);

    return 0;
}

void SoilMoistureSensor::setCalibration(int16_t dry, int16_t wet)
{
    dry_raw_ = dry;
    wet_raw_ = wet;
}
