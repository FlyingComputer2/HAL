#include <halsim/halsim.hpp>

namespace hal
{

class RTCSPI: ISPIHandler
{
    ~RTCSPI() = default;
    void start() override;
    void stop() override;
    void xfer(const std::byte*, std::byte*, size_t) override;
};

class RTCI2C : II2CHandler
{
    ~RTCI2C() = default;
    void start() override;
    void stop() override;
    void xfer(const std::byte*, std::byte*, size_t) override;
};

} // namespace hal
