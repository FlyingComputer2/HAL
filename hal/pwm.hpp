#ifndef __HAL_PWM_HPP__
#define __HAL_PWM_HPP__

#include <optional>
#include <cstdint>
#include <string>

namespace hal
{

struct pwm_config_s
{
    enum alignment_e
    {
        PWM_LEFT_ALIGNED,
        PWM_CENTER_ALIGNED,
        PWM_RIGHT_ALIGNED
    };

    uint64_t frequency_hz;
    uint8_t resolution_bits;
    alignment_e alignment;
    bool enable_invert_polarity;
};

struct pwm_id_s
{
    std::string name;
    std::optional<uint8_t> pin_out;
    std::optional<uint8_t> channel;
};

using pwm_t = void*;

pwm_t pwm_init(const pwm_id_s& id, const pwm_config_s& config);
void pwm_deinit(pwm_t pwm);
void pwm_reconfigure(pwm_t pwm, const pwm_config_s& config);
void pwm_set_duty_cycle(pwm_t pwm, uint64_t duty_cycle);
void pwm_start(pwm_t pwm);
void pwm_stop(pwm_t pwm);
void pwm_set_phase_shift(pwm_t pwm, uint64_t phase_shift);
void pwm_sync(pwm_t master, pwm_t slave);

} // namespace hal

#endif // __HAL_PWM_HPP__
