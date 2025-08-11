#ifndef __HAL_GPIO_HPP__
#define __HAL_GPIO_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct gpio_config_s
{
    enum direction_e
    {
        GPIO_INPUT,
        GPIO_OUTPUT
    };

    enum pull_mode_e
    {
        GPIO_FLOATING,
        GPIO_PULL_UP,
        GPIO_PULL_DOWN
    };

    enum edge_trigger_e
    {
        GPIO_EDGE_NONE,
        GPIO_EDGE_RISING,
        GPIO_EDGE_FALLING,
        GPIO_EDGE_BOTH
    };

    direction_e direction;
    pull_mode_e pull_mode;
    edge_trigger_e interrupt_trigger;
};

struct gpio_id_s
{
    std::string name;
    std::optional<uint8_t> pin;
};

using gpio_t = void*;

gpio_t gpio_init(const gpio_id_s& id, const gpio_config_s& config);
void gpio_deinit(gpio_t gpio);
void gpio_write(gpio_t gpio, bool state);
bool gpio_read(gpio_t gpio);
void gpio_set_direction(gpio_t gpio, gpio_config_s::direction_e dir);
void gpio_set_interrupt_callback(gpio_t gpio, void (*callback)(void*), void* user_data);
void gpio_clear_interrupt(gpio_t gpio);

} // namespace hal

#endif // __HAL_GPIO_HPP__
