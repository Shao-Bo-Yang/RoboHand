#pragma once

#include "robo_task.h"
namespace task
{

struct pwm_led_task : public task_generic<pwm_led_task>
{
  public:
    void operator()();

  private:
    pwm_led_task() = default;
    ~pwm_led_task() = default;

  private:
    friend task_generic;
};

} // namespace task
