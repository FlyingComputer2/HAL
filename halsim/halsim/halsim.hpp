#ifndef __HALSIM_HAL_SERVER_HAL_HANDLER_HPP__
#define __HALSIM_HAL_SERVER_HAL_HANDLER_HPP__

#include "ipc/sync.hpp"

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

// Implemented when used with dynamic linking
extern "C" std::shared_ptr<ISPIHandler> getSPIHandler();
extern "C" std::shared_ptr<II2CHandler> getII2CHandler();

} // namespace hal

#endif // __HALSIM_HAL_SERVER_HAL_HANDLER_HPP__
