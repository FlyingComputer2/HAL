#ifndef __HAL_PROTOCOL_HPP__
#define __HAL_PROTOCOL_HPP__

#include <cstdint>
#include <cstddef>

namespace hal
{
namespace protocol
{

enum msg_e : uint8_t
{
    E_MSG_SPI_STATUS_CODE,
    E_MSG_SPI_ACQUIRE_BUS,
    E_MSG_SPI_RELEASE_BUS,
    E_MSG_SPI_XFER_IN,
    E_MSG_SPI_XFER_OUT,
    E_MSG_I2C_STATUS_CODE,
    E_MSG_I2C_START,
    E_MSG_I2C_STOP,
    E_MSG_I2C_WRITE,
    E_MSG_I2C_READ,
    E_MSG_I2C_READ_DATA
};

struct header_s
{
    msg_e type;
    uint8_t spare0;
    uint16_t sequencer_number;
};

/*******************************************************************************
 * 
 * SPI Messages Definition
 * 
 *   Request/Response structure:
 *     msg_spi_aquire_bus_s  --> msg_spi_status_code_s
 *     msg_spi_release_bus_s --> msg_spi_status_code_s
 *     msg_spi_xfer_in_s     --> (msg_spi_xfer_out_s | msg_spi_status_code_s)
 * 
*******************************************************************************/

struct msg_spi_status_code_s
{
    enum status_e : uint8_t
    {
        E_MSG_SPI_OK,
        E_MSG_SPI_INVALID_DEVICE,
        E_MSG_SPI_BUS_BUSY,
        E_MSG_SPI_BUS_NOT_ACQUIRED
    };
    
    header_s header{msg_e::E_MSG_SPI_STATUS_CODE, 0};
    status_e status;
};

/**
 * @brief Aquires bus by asserting CS pin indicated by chip
 *   Responses:
 *     E_MSG_SPI_OK             - Bus is acquired
 *     E_MSG_SPI_BUS_BUSY       - Bus is already acquired
 *     E_MSG_SPI_INVALID_DEVICE - Device is not found
 */
struct msg_spi_acquire_bus_s
{
    header_s header{msg_e::E_MSG_SPI_ACQUIRE_BUS, 0};
    uint8_t bus;
    uint8_t chip;
};

/**
 * @brief Release bus by deasserting CS pin on the bus
 *   Responses:
 *     E_MSG_SPI_OK               - Bus is released.
 *     E_MSG_SPI_BUS_NOT_ACQUIRED - Bus has not been acquired previously.
 */
struct msg_spi_release_bus_s
{
    header_s header{msg_e::E_MSG_SPI_RELEASE_BUS, 0};
    uint8_t bus;
};

/**
 * @brief Start xfer operation on bus.
 *   Responses:
 *     msg_spi_xfer_out_s         - Xfer output.
 *     E_MSG_SPI_BUS_NOT_ACQUIRED - Bus has not been acquired previously.
 */
struct msg_spi_xfer_in_s
{
    header_s header{msg_e::E_MSG_SPI_XFER_IN, 0};
    uint8_t bus;
    uint16_t size;

    std::byte* data()
    {
        return (std::byte*)this + sizeof(msg_spi_xfer_in_s);
    }
};

struct msg_spi_xfer_out_s
{
    header_s header{msg_e::E_MSG_SPI_XFER_OUT, 0};
    uint8_t bus;
    uint16_t size;

    std::byte* data()
    {
        return (std::byte*)this + sizeof(msg_spi_xfer_out_s);
    }
};

/*******************************************************************************
 * 
 * I2C Messages Definition
 * 
 *   Request/Response Structure
 *     msg_i2c_start_s  --> msg_i2c_status_code_s
 *     msg_i2c_stop_s   --> msg_i2c_status_code_s
 *     msg_i2c_write_s  --> msg_i2c_status_code_s
 *     msg_i2c_read_s   --> msg_i2c_status_code_s, msg_i2c_read_data_s
 * 
*******************************************************************************/

struct msg_i2c_status_code_s
{
    enum status_e : uint8_t
    {
        E_MSG_I2C_OK,
        E_MSG_I2C_INVALID_DEVICE,
        E_MSG_I2C_BUS_BUSY,
        E_MSG_I2C_BUS_NO_AQUIRED,
        E_MSG_I2C_UNEXPECTED_STOP,
    };

    msg_e type = msg_e::E_MSG_I2C_STATUS_CODE;
    status_e status;
};

struct msg_i2c_start_s
{
    msg_e type = msg_e::E_MSG_I2C_START;
    uint8_t bus;
    uint16_t address;
};

struct msg_i2c_stop_s
{
    msg_e type = msg_e::E_MSG_I2C_STOP;
    uint8_t bus;
};

struct msg_i2c_write_s
{
    msg_e type = msg_e::E_MSG_I2C_WRITE;
    uint8_t bus;
    uint16_t data_size;
    std::byte data[1020];
};

struct msg_i2c_read_s
{
    msg_e type = msg_e::E_MSG_I2C_READ;
    uint8_t bus;
    uint16_t data_size;
};

struct msg_i2c_read_data_s
{
    msg_e type = msg_e::E_MSG_I2C_READ_DATA;
    uint8_t bus;
    uint16_t data_size;

    std::byte* data()
    {
        return (std::byte*)this + sizeof(msg_i2c_read_data_s);
    }
};

} // namespace protocol
} // namespace hal

#endif // __HALSERVER_PROTOCOL_HPP__
