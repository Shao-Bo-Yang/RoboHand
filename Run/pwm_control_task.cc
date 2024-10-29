#include "pwm_control_task.h"
#include "app_time.h"
#include "bezier.hh"
#include "pwm.h"
#include <algorithm>
#include <array>
#include <cmsis_os2.h>
#include <cstdint>
#include <vector>

namespace task
{

static constexpr std::array<bsd::pwm_channel, BSD_PWM_CHANNEL_COUNT> pwm_channels = {
    bsd::pwm_channel::Ch1, bsd::pwm_channel::Ch2, bsd::pwm_channel::Ch3, bsd::pwm_channel::Ch4,
    bsd::pwm_channel::Ch5, bsd::pwm_channel::Ch6, bsd::pwm_channel::Ch7, bsd::pwm_channel::Ch8,
};

pwm_control_task::~pwm_control_task()
{
    osSemaphoreDelete(_pwm_channel_controls_sema);
}

pwm_control_task::pwm_control_task() : _pwm_channel_controls(), _pwm_channel_controls_sema(osSemaphoreNew(1, 1, NULL))
{
    _control_points = {
        base::point{0, 0},
        base::point{0.42, 0},
        base::point{0.58, 1},
        base::point{1, 1},
    };
    auto delta = 1.0 / _route.size();
    std::vector control_point_vect(_control_points.begin(), _control_points.end());
    for (int32_t i = 0; i < (int32_t)_route.size(); ++i)
    {
        base::bezier(control_point_vect, delta * i, _route[i]);
    }

    for (int32_t i = 0; i < (int32_t)pwm_channels.size(); ++i)
    {
        auto ch = pwm_channels[i];
        _pwm_channel_controls[i] = {
            ch, 0, 0, 0, 2, false,
        };
    }
}

void pwm_control_task::operator()()
{

    for (;;)
    {
        auto cur_tick = bsd::app_time::app_time_ms();
        osSemaphoreAcquire(_pwm_channel_controls_sema, osWaitForever);
        for (auto &control : _pwm_channel_controls)
        {
            if (!control.enable)
            {
                continue;
            }
            auto destination_tick = control.start_tick_count + uint64_t(control.duration * 1000);
            if (cur_tick > destination_tick)
            {
                bsd::pwm::use_device(
                    [&control](bsd::pwm &device) { device.set_duty(control.destination_duty, control.ch); });
                control.enable = false;
            }
            else
            {
                auto delta = cur_tick - control.start_tick_count;
                auto ratio = delta / (control.duration * 1000);
                auto finded = std::lower_bound(_route.begin(), _route.end(), ratio,
                                               [](const base::point &a, const float &b) { return a.x < b; });
                float duty = 0;
                if (finded == _route.end())
                {
                    duty = control.destination_duty;
                }
                else
                {
                    duty = (control.destination_duty - control.recorded_duty) *
                               std::min(std::max(finded->y, float(0.0)), float(1.0)) +
                           control.recorded_duty;
                }
                bsd::pwm::use_device([duty, &control](bsd::pwm &device) { device.set_duty(duty, control.ch); });
            }
        }
        osSemaphoreRelease(_pwm_channel_controls_sema);
        osDelay(1);
    }
}

} // namespace task
