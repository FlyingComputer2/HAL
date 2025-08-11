#ifndef __HAL_I2C_HPP__
#define __HAL_I2C_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct i2c_config_s
{
    enum address_size_e
    {
        I2C_7BIT_ADDRESS,
        I2C_10BIT_ADDRESS
    };

    constexpr static uint32_t I2C_STANDARD_MODE   =  100000; // 100 kHz
    constexpr static uint32_t I2C_FAST_MODE       =  400000; // 400 kHz
    constexpr static uint32_t I2C_FAST_MODE_PLUS  = 1000000; // 1   MHz
    constexpr static uint32_t I2C_HIGH_SPEED_MODE = 3400000; // 3.4 MHz

    uint32_t clock_speed_hz;
    address_size_e address_size;
    bool enable_clock_stretching;
};

struct i2c_id_s
{
    std::string name;
    std::optional<uint8_t> bus;
    std::optional<uint8_t> pin_sda;
    std::optional<uint8_t> pin_sck;
};

using i2c_t = void*;

i2c_t i2c_init(const i2c_id_s& id, const i2c_config_s& config);
void i2c_deinit(i2c_t master);
int  i2c_start(i2c_t master, uint16_t address);
int  i2c_stop(i2c_t master);
int  i2c_read(i2c_t master, std::byte *data, size_t count);
int  i2c_write(i2c_t master, const std::byte *data, size_t count);

} // namespace hal

#endif // __HAL_I2C_HPP__
