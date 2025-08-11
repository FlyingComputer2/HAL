#ifndef __HAL_ADC_HPP__
#define __HAL_ADC_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct adc_config_s
{
    enum reference_e
    {
        ADC_REF_VCC,
        ADC_REF_INTERNAL,
        ADC_REF_EXTERNAL
    };

    uint32_t sample_rate_hz;
    uint8_t resolution_bits;
    reference_e voltage_reference;
};

struct adc_id_s
{
    std::string name;
    std::optional<uint8_t> channel;
    std::optional<uint8_t> pin;
};

using adc_t = void*;

adc_t adc_init(const adc_id_s& id, const adc_config_s& config);
void adc_deinit(adc_t adc);
uint64_t adc_read(adc_t adc);
void adc_read(adc_t adc, void (*callback)(float voltage, void* user_data), void* user_data);

} // namespace hal

#endif // __HAL_ADC_HPP__