#pragma once

#include <array>
#include <cmath>
#include <stdint.h>

#include "app_time.h"
#include "bezier.hh"
#include "log.hh"
#include "pwm.h"
#include "robo_task.h"
#include <cmsis_os2.h>
namespace task
{

struct pwm_control_task : public task_generic<pwm_control_task>
{
  public:
    void operator()();

    template <bsd::pwm_channel ch> void set_duty(float duty)
    {
        osSemaphoreAcquire(_pwm_channel_controls_sema, osWaitForever);
        auto &controller = _pwm_channel_controls[(uint32_t)ch - 1];
        controller.enable = true;
        bsd::pwm::use_device([&](bsd::pwm &device) { controller.recorded_duty = device.get_duty<ch>(); });
        auto delta = std::fabs(controller.recorded_duty - duty);
        auto duration = delta * 10 * 2;
        controller.duration = std::min(std::max(float(0.0), duration), float(3.0));
        controller.start_tick_count = bsd::app_time::app_time_ms();
        controller.destination_duty = duty;
        osSemaphoreRelease(_pwm_channel_controls_sema);
        base::log("Soft Set Duty{ ch: ", (uint32_t)ch, ", duty: ", int(duty * 100), " }");
    }

  private:
    struct _pwm_channel_control
    {
        bsd::pwm_channel ch;

        float recorded_duty;
        float destination_duty;
        uint64_t start_tick_count;
        float duration;
        bool enable;
    };

    std::array<_pwm_channel_control, BSD_PWM_CHANNEL_COUNT> _pwm_channel_controls;

    osSemaphoreId_t _pwm_channel_controls_sema;
    std::array<base::point, 4> _control_points;
    std::array<base::point, 500> _route;

  private:
    pwm_control_task();
    ~pwm_control_task();

  private:
    friend task_generic;
};

} // namespace task
