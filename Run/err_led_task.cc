#include "err_led_task.h"

#include <memory>

#include "cmsis_os2.h"
#include "led.h"
#include "robo_task.h"

namespace task {

err_led_task::err_led_task() : _err_flag_event(osEventFlagsNew(NULL)) {}

err_led_task::~err_led_task() { osEventFlagsDelete(_err_flag_event); }

void err_led_task::operator()() {
  using bsd::led;
  led::use_device(
      [](led& led) { led.switch_led<bsd::signal_led_type::Err, false>(); });
  osEventFlagsWait(_err_flag_event, osFlagsWaitAny | osFlagsNoClear, 0,
                   osWaitForever);
  led::use_device(
      [](led& led) { led.switch_led<bsd::signal_led_type::Err, true>(); });
  osThreadTerminate(get_task_id());
}

err_flags err_led_task::get_err() { return osEventFlagsGet(_err_flag_event); }

void err_led_task::set_err(err_flags flags) {
  osEventFlagsSet(_err_flag_event, flags.data);
}

}  // namespace task
