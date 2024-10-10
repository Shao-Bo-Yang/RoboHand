#include "root_task.h"

#include <cstddef>
#include <cstdint>

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "err_led_task.h"
#include "flash.h"
#include "led.h"
#include "main.h"
#include "pwm.h"
#include "pwm_led_task.h"
#include "task.h"
#include "test_task.h"
#include "usart_log_task.h"
#include "work_led_task.h"

void root_task_handler(void* argument) {
  taskENTER_CRITICAL();
  using namespace bsd;
  using namespace task;
  pwm::use_device([](pwm& device) { device.init(); });
  flash::use_device([](flash& device) { device.init(); });
  led::use_device([](led& device) { device.init(); });

  pwm::use_device([](pwm& device) {
    device.set_duty<bsd::pwm_channel::Ch1>(0.1);
    device.set_duty<bsd::pwm_channel::Ch2>(0.15);
    device.set_duty<bsd::pwm_channel::Ch3>(0.2);
    device.set_duty<bsd::pwm_channel::Ch4>(0.25);
    device.set_duty<bsd::pwm_channel::Ch5>(0.3);
    device.set_duty<bsd::pwm_channel::Ch6>(0.35);
    device.set_duty<bsd::pwm_channel::Ch7>(0.4);
    device.set_duty<bsd::pwm_channel::Ch8>(0.45);
  });

  pwm_led_task::instance().init("pwm_led_task", osPriorityLow);
  work_led_task::instance().init("work_led_task", osPriorityLow);
  err_led_task::instance().init("err_led_task", osPriorityLow);
  usart_log_task::instance().init("usart_log_task", osPriorityLow);
  test_task::instance().init("test_task", osPriorityLow);

  taskEXIT_CRITICAL();

  osThreadTerminate(osThreadGetId());
}
