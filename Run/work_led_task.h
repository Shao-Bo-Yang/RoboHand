#pragma once

#include "robo_task.h"
#include <cmsis_os2.h>
namespace task
{

struct work_led_task : public task_generic<work_led_task>
{
  public:
    void operator()();

    void blink();

  private:
    work_led_task();
    ~work_led_task();

  private:
    osSemaphoreId_t _notification;
    friend task_generic;
};

} // namespace task
