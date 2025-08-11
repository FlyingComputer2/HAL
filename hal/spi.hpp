#ifndef __HAL_SPI_HPP__
#define __HAL_SPI_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct spi_config_s
{
    enum mode_e : uint8_t
    {
        MODE0, // CPOL=0, CPHA=0
        MODE1, // CPOL=0, CPHA=1
        MODE2, // CPOL=1, CPHA=0
        MODE3  // CPOL=1, CPHA=1
    };

    enum bit_order_e : uint8_t
    {
        BIT_ORDER_MSB = 0,
        BIT_ORDER_LSB
    };

    uint32_t clock_speed_hz;
    mode_e mode;
    bit_order_e bit_order;
    uint8_t data_bits;
    bool cs_active_low;
};

struct spi_id_s
{
    std::string name;
    std::optional<uint8_t> bus;
    std::optional<uint8_t> chip;
    std::optional<uint8_t> pin_cs;
};

using spi_t = void*;

spi_t spi_init(const spi_id_s& id, const spi_config_s& config);
int spi_deinit(spi_t spi);
int spi_transfer(spi_t spi, const std::byte *tx_data, std::byte *rx_data, size_t count);
int spi_write(spi_t spi, const std::byte *data, size_t count);
int spi_read(spi_t spi, std::byte *data, size_t count);

} // namespace hal

#endif // __HAL_SPI_HPP__
