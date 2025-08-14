#include <mutex>
#include <regex>

#include <bfc/udp.hpp>
#include <bfc/configuration_parser.hpp>

#include <hal/spi.hpp>
#include <halserver/protocol.hpp>

namespace hal
{

std::pair<unsigned, unsigned> parseBusChip(const std::string& input)
{
    std::pair<unsigned, unsigned> rv{0,0};
    std::regex pattern(R"(([^0-9]+)([0-9]+).([0-9]+))");
    std::smatch matches;
    if (!std::regex_match(input, matches, pattern))
    {
        return rv;
    }
    if (matches.size() < 4)
    {
        return rv;
    }
    try
    {
        unsigned long first = std::stoul(matches[2].str());
        unsigned long second = std::stoul(matches[3].str());
        return {static_cast<unsigned>(first), static_cast<unsigned>(second)};
    }
    catch (const std::exception& e)
    {
        return rv;
    }
}

struct spi_impl_t
{
    sockaddr_in serveraddr;
    bfc::socket socket;
    uint8_t bus = 0;
    uint8_t chip = 0;

    uint16_t sequence_number = 0;
    bool is_nonblocking = true;

    std::mutex m;
};

spi_t spi_init(const spi_id_s& id, const spi_config_s& config)
{
    auto [named_bus, named_chip] = parseBusChip(id.name);

    auto expected_bus  = id.bus.value_or(named_bus);
    auto expected_chip = id.chip.value_or(named_chip);

    bfc::configuration_parser args;
    args.load("hal.cfg");
    auto n = args.as<size_t>("devices.size").value_or(0);

    std::string host;
    uint16_t port = 0;
    uint8_t  bus  = 0;
    uint8_t  chip = 0;

    for (auto i=0u; i<n; i++)
    {
        auto base  = "devices[" + std::to_string(i) + "]";
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
            bus  = vbus;
            chip = vchip;
            break;
        }
    }

    if (host.empty())
    {
        return nullptr;
    }

    auto rv = new spi_impl_t{};
    rv->serveraddr = bfc::ip4_port_to_sockaddr(host, port);
    rv->socket     = bfc::create_udp4();
    rv->bus        = bus;
    rv->chip       = chip;


    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    if (setsockopt(rv->socket.fd(), SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        rv->is_nonblocking = false;
    }

    return rv;
}

void spi_deinit(spi_t s)
{
    auto spi = (spi_impl_t*)s;
    delete spi;
}

int spi_transfer(spi_t spi_, const std::byte *tx_data, std::byte *rx_data, size_t count)
{
    auto& spi = *(spi_impl_t*)(spi_);
    std::unique_lock lg(spi.m);

    std::byte rcv[1024+sizeof(protocol::msg_spi_xfer_out_s)];
    bfc::buffer_view bvrcv(&rcv, sizeof(rcv));

    protocol::msg_spi_acquire_bus_s acquire{};
    acquire.bus  = spi.bus;
    acquire.chip = spi.chip;
    acquire.header.sequencer_number = spi.sequence_number++;

    while (true)
    {
        if (0 > spi.socket.send(bfc::const_buffer_view(&acquire, sizeof(acquire))), 0, spi.serveraddr, sizeof(spi.serveraddr))
        {
            if (EINTR == errno)
            {
                continue;
            }
            return -1;
        }

        auto rv = spi.socket.recv(bvrcv, 0);

        if (0 > rv && EINTR != errno)
        {
            return -1;
        }

        const auto& rspheader = *(protocol::header_s*)(rcv);
        if (rspheader.type != protocol::E_MSG_SPI_STATUS_CODE)
        {
            errno = EPROTO;
            return -1;
        }
        
        const auto& rspstatus = *(protocol::msg_spi_status_code_s*)(rcv);

        if (protocol::msg_spi_status_code_s::E_MSG_SPI_BUS_BUSY == rspstatus.status)
        {
            errno = EBUSY;
            return -1;
        }

        if (protocol::msg_spi_status_code_s::E_MSG_SPI_INVALID_DEVICE == rspstatus.status)
        {
            errno = ENODEV;
            return -1;
        }

        if (protocol::msg_spi_status_code_s::E_MSG_SPI_OK == rspstatus.status)
        {
            break;
        }
    }

    while (count)
    {
        
    }
}


} // namespace hal
