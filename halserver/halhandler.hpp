#ifndef __HALSERVER_HALHANDLER_HPP__
#define __HALSERVER_HALHANDLER_HPP__

#include <memory>
#include <cstdint>

namespace hal
{

struct ISPIHandler
{
    ISPIHandler() = delete;
    ISPIHandler(uint8_t bus, uint8_t chip);
    ~ISPIHandler() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void xfer(const std::byte*, std::byte*, size_t) = 0;
};

struct II2CHandler
{
    II2CHandler() = delete;
    II2CHandler(uint8_t bus, uint16_t address);
    ~II2CHandler() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void xfer(const std::byte*, std::byte*, size_t) = 0;
};

} // namespace hal

#endif // __HALSERVER_HALHANDLER_HPP__
