#pragma once

#include <functional>
#include <string>
#include <vector>

#include "pwm.h"
#include "pwm_control_task.h"
namespace command_tasks
{

void update_pwm_value(const std::vector<std::string> &args);

void store_pwm_value(const std::vector<std::string> &args);

void _set_angle_task(const std::vector<std::string> &args, std::function<void(float duty)> func, bsd::pwm_channel ch);

template <bsd::pwm_channel ch> void set_angle_task(const std::vector<std::string> &args)
{
    auto func = [](float duty) { task::pwm_control_task::instance().set_duty<ch>(duty); };
    return _set_angle_task(args, func, ch);
}

void _set_pwm_duty_task(const std::vector<std::string> &args, std::function<void(float duty)> func,
                        bsd::pwm_channel ch);

template <bsd::pwm_channel ch> void set_pwm_duty_task(const std::vector<std::string> &args)
{

    auto func = [](float duty) { task::pwm_control_task::instance().set_duty<ch>(duty); };
    return _set_pwm_duty_task(args, func, ch);
}

} // namespace command_tasks
