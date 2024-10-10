#include "pwm_led_task.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "cmsis_os2.h"
#include "led.h"
#include "pwm.h"
#include "robo_task.h"

namespace {

using ch = bsd::pwm_channel;
static constexpr ch ch_list[]{ch::Ch1, ch::Ch2, ch::Ch3, ch::Ch4,
                              ch::Ch5, ch::Ch6, ch::Ch7, ch::Ch8};

template <int32_t _led_begin, int32_t _led_end,
          typename std::enable_if<(_led_begin > _led_end), bool>::type = true>
static auto pwm_led_blinky(bsd::pwm&, bsd::led&, uint32_t, uint32_t, ...) {}

template <int32_t _led_begin, int32_t _led_end,
          typename std::enable_if<(_led_begin <= _led_end), bool>::type = true>
static auto pwm_led_blinky(bsd::pwm& pwm, bsd::led& led, uint32_t times,
                           uint32_t scale) {
  constexpr auto ch = ch_list[_led_begin - 1];

  double duty = pwm.get_duty<ch>();
  if (times < duty * scale) {
    led.switch_led<_led_begin, true>();

  } else {
    led.switch_led<_led_begin, false>();
  }
  return pwm_led_blinky<_led_begin + 1, _led_end>(pwm, led, times, scale);
}

}  // namespace

namespace task {

void pwm_led_task::operator()() {
  uint32_t times = 0;
  const uint32_t scale = 1000;
  using bsd::led;
  using bsd::pwm;
  for (;;) {
    pwm::use_device([&](pwm& pwm) {
      led::use_device([&](led& led) {
        pwm_led_blinky<1, 8>(pwm, led, times, scale);
        times += 1;
        times = times % scale;
      });
    });
    osDelay(1);
  }
}

}  // namespace task
