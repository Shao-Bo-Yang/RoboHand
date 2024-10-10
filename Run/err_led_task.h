#pragma once

#include <cstdint>
#include <memory>

#include "cmsis_os2.h"
#include "robo_task.h"
namespace task {

union err_flags {
  struct _ {
    bool init_err : 1;
    bool pwm_err : 1;
  };
  uint32_t data;

  err_flags() : data(0) {}
  err_flags(uint32_t flags) : data(flags) {};
};

struct err_led_task : public task_generic<err_led_task> {
 public:
  void operator()();

  void set_err(err_flags flags);

  err_flags get_err();

 private:
  err_led_task();
  ~err_led_task();

 private:
  osEventFlagsId_t _err_flag_event;
  friend task_generic;
};

}  // namespace task
