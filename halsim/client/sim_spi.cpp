#ifndef __CLIENT_SIM_SPI_HPP__
#define __CLIENT_SIM_SPI_HPP__

#include <hal/spi.hpp>

#include "ipc/ipc_cmds.hpp"
#include "ipc/ipc.hpp"

namespace hal
{

struct spi_context_s
{
};

spi_t spi_init(const spi_id_s& id, const spi_config_s& config)
{
    return nullptr;
}

void spi_deinit(spi_t spi)
{
}

int  spi_transfer(spi_t spi, const std::byte *tx_data, std::byte *rx_data, size_t count)
{
    return 0;
}

int  spi_write(spi_t spi, const std::byte *data, size_t count)
{
    return 0;
}

int  spi_read(spi_t spi, std::byte *data, size_t count)
{
    return 0;
}

} // namespace hal

#endif // __CLIENT_SIM_SPI_HPP__
