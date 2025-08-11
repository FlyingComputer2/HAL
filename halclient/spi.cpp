#include <bfc/udp.hpp>
#include <bfc/configuration_parser.hpp>

#include <hal/spi.hpp>
#include <halserver/protocol.hpp>

namespace hal
{

struct spi_impl_t
{
    bfc::udp_socket sock;
};

spi_t spi_init(const spi_id_s& id, const spi_config_s& config)
{
    bfc::configuration_parser args;
    args.load("halcllient.cfg");
    args.as<size_t>("").value_or(0);
}

int spi_deinit(spi_t spi)
{

}

int spi_transfer(spi_t spi, const std::byte *tx_data, std::byte *rx_data, size_t count);
int spi_write(spi_t spi, const std::byte *data, size_t count);
int spi_read(spi_t spi, std::byte *data, size_t count);

} // namespace hal
