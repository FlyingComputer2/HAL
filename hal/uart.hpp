#ifndef __HAL_UART_HPP__
#define __HAL_UART_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct uart_config_s
{
    uint32_t baud;
    bool inverted;
};

struct uart_id_s
{
    std::string name;
    std::optional<uint8_t> pin_tx;
    std::optional<uint8_t> pin_rx;
};

using uart_t = void*;

uart_t uart_init(const uart_t& id, const uart_id_s& config);
void uart_deinit(uart_t uart);
int  uart_tx(uart_t uart, std::byte *data, size_t count);
int  uart_rx(uart_t uart, const std::byte *data, size_t count);

} // namespace hal

#endif // __HAL_I2C_HPP__
