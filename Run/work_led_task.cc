#include "work_led_task.h"

#include <cstdint>
#include <limits>
#include <memory>

#include "cmsis_os2.h"
#include "led.h"
#include "robo_task.h"

namespace task {

work_led_task::work_led_task()
    : _notification(
          osSemaphoreNew(std::numeric_limits<uint32_t>::max(), 0, NULL)) {}

work_led_task::~work_led_task() { osSemaphoreDelete(_notification); }

void work_led_task::operator()() {
  using bsd::led;
  led::use_device(
      [&](led &_led) { _led.switch_led<bsd::signal_led_type::Work, true>(); });
  for (;;) {
    osSemaphoreAcquire(_notification, osWaitForever);
    led::use_device([&](led &_led) {
      _led.switch_led<bsd::signal_led_type::Work, false>();
    });
    osDelay(100);
    led::use_device([&](led &_led) {
      _led.switch_led<bsd::signal_led_type::Work, true>();
    });
  }
}

void work_led_task::blink() {
  if (osThreadGetId() != get_task_id()) {
    osSemaphoreRelease(_notification);
  }
}

}  // namespace task
