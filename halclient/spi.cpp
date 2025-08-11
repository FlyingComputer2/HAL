#include <bfc/udp.hpp>
#include <bfc/configuration_parser.hpp>

#include <hal/spi.hpp>
#include <halserver/protocol.hpp>

namespace hal
{

struct spi_impl_t
{
    sockaddr_in serveraddr;
    bfc::socket sock;
};

spi_t spi_init(const spi_id_s& id, const spi_config_s& config)
{
    uint8_t named_bus;
    uint8_t named_chip;

    auto expected_bus  = id.chip.value_or(named_bus);
    auto expected_chip = id.chip.value_or(named_chip);

    bfc::configuration_parser args;
    args.load("halcllient.cfg");
    auto n = args.as<size_t>("devices.size").value_or(0);

    std::string host;
    uint16_t port;

    for (auto i=0u; i<n; i++)
    {
        auto base = "devices[" + std::to_string(i) + "]";
        auto khost = base+".remote.host";
        auto kport = base+".remote.port";
        auto kbus  = base+".bus";
        auto kchip = base+".chip";

        auto vbus  = args.as<uint8_t>(kbus).value_or(0);
        auto vchip = args.as<uint8_t>(kchip).value_or(0);
        auto vhost = args.arg(khost).value_or("");
        auto vport = args.as<uint16_t>(kport).value_or(0);
        
        if (expected_bus == vbus && expected_chip == vchip)
        {
            host = vhost;
            port = vport;
            break;
        }
    }

    if (host.empty())
    {
        return nullptr;
    }

    auto rv = new spi_impl_t{};
    rv->serveraddr = bfc::ip4_port_to_sockaddr(host, port);

    return rv;
}

void spi_deinit(spi_t s)
{
    auto spi = (spi_impl_t*)s;
    delete spi;
}

int spi_transfer(spi_t spi, const std::byte *tx_data, std::byte *rx_data, size_t count);
int spi_write(spi_t spi, const std::byte *data, size_t count);
int spi_read(spi_t spi, std::byte *data, size_t count);

} // namespace hal
