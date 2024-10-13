#pragma once

#include "bsd_getter.h"
#include <FreeRTOS.h>
#include <task.h>

namespace bsd {

struct app_time : public bsd_getter<app_time> {
public:
  void init();
  static uint64_t app_time_ms(void);

private:
  struct _critical {

    _critical() { taskENTER_CRITICAL(); }
    ~_critical() { taskEXIT_CRITICAL(); }
  };
};

} // namespace bsd
