#ifndef __CLIENT_SIM_I2C_HPP__
#define __CLIENT_SIM_I2C_HPP__

#include <hal/i2c.hpp>

#include "ipc/ipc_cmds.hpp"
#include "ipc/ipc.hpp"

namespace hal
{

struct i2c_context_s
{
};

i2c_t i2c_init(const i2c_id_s& id, const i2c_config_s& config)
{
    return nullptr;
}

void i2c_deinit(i2c_t master)
{}

int  i2c_start(i2c_t master, uint16_t address)
{}

int  i2c_stop(i2c_t master)
{}

int  i2c_read(i2c_t master, std::byte *data, size_t count)
{}

int  i2c_write(i2c_t master, const std::byte *data, size_t count)
{}


} // namespace hal

#endif // __CLIENT_SIM_I2C_HPP__
