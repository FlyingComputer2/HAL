#ifndef __HALSIM_IPC_CMDS_HPP__
#define __HALSIM_IPC_CMDS_HPP__

#include "ipc/sync.hpp"
#include "hal/spi.hpp"

namespace hal
{

enum cmd_e : uint8_t
{
    E_CMD_SPI_STATUS_CODE,
    E_CMD_SPI_ACQUIRE_BUS,
    E_CMD_SPI_RELEASE_BUS,
    E_CMD_SPI_XFER,
    E_CMD_I2C_STATUS_CODE,
    E_CMD_I2C_START,
    E_CMD_I2C_STOP,
    E_CMD_I2C_WRITE,
    E_CMD_I2C_READ,
    E_CMD_I2C_READ_DATA
};

struct cmd_spi_status_code_s
{
    enum status_e : uint8_t
    {
        E_CMD_SPI_OK,
        E_CMD_SPI_INVALID_DEVICE,
        E_CMD_SPI_BUS_BUSY,
    };
    
    cmd_e type = cmd_e::E_CMD_SPI_STATUS_CODE;
    status_e status;
};

struct cmd_spi_acquire_bus_s
{
    cmd_e type = cmd_e::E_CMD_SPI_ACQUIRE_BUS;
    uint8_t bus;
    uint8_t chip;
};

struct cmd_spi_release_bus_s
{
    cmd_e type = cmd_e::E_CMD_SPI_RELEASE_BUS;
    uint8_t bus;
};

struct cmd_spi_xfer_s
{
    cmd_e type = cmd_e::E_CMD_SPI_XFER;
    uint8_t bus;
    uint16_t size;
    std::byte data[1020];
};


// cmd_spi_aquire_bus_s  --> cmd_spi_status_code_s
// cmd_spi_release_bus_s --> cmd_spi_status_code_s
// cmd_spi_xfer_s        --> cmd_spi_xfer_s

static_assert(1024 >= sizeof(cmd_spi_status_code_s));
static_assert(1024 >= sizeof(cmd_spi_aquire_bus_s));
static_assert(1024 >= sizeof(cmd_spi_release_bus_s));
static_assert(1024 >= sizeof(cmd_spi_xfer_s));

struct cmd_i2c_status_code_s
{
    enum status_e : uint8_t
    {
        E_CMD_I2C_OK,
        E_CMD_I2C_INVALID_DEVICE,
        E_CMD_I2C_BUS_BUSY,
        E_CMD_I2C_UNEXPECTED_STOP,
    };

    cmd_e type = cmd_e::E_CMD_I2C_STATUS_CODE;
    status_e status;
};

struct cmd_i2c_start_s
{
    cmd_e type = cmd_e::E_CMD_I2C_START;
    uint8_t bus;
    uint16_t address;
};

struct cmd_i2c_stop_s
{
    cmd_e type = cmd_e::E_CMD_I2C_STOP;
    uint8_t bus;
};

struct cmd_i2c_write_s
{
    cmd_e type = cmd_e::E_CMD_I2C_WRITE;
    uint8_t bus;
    uint16_t data_size;
    std::byte data[1020];
};

struct cmd_i2c_read_s
{
    cmd_e type = cmd_e::E_CMD_I2C_READ;
    uint8_t bus;
    uint16_t data_size;
};

struct cmd_i2c_read_data_s
{
    cmd_e type = cmd_e::E_CMD_I2C_READ;
    uint8_t bus;
    uint16_t data_size;
    std::byte data[1020];
};

// cmd_i2c_start_s  --> cmd_i2c_status_code_s
// cmd_i2c_stop_s   --> cmd_i2c_status_code_s
// cmd_i2c_write_s  --> cmd_i2c_status_code_s
// cmd_i2c_read_s   --> cmd_i2c_status_code_s, cmd_i2c_read_data_s

static_assert(1024 >= sizeof(cmd_i2c_status_code_s));
static_assert(1024 >= sizeof(cmd_i2c_start_s));
static_assert(1024 >= sizeof(cmd_i2c_stop_s));
static_assert(1024 >= sizeof(cmd_i2c_read_s));
static_assert(1024 >= sizeof(cmd_i2c_write_s));

} // namespace hal

#endif // __HALSIM_IPC_CMDS_HPP__
