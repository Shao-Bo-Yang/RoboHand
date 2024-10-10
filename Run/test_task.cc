#include "test_task.h"

#include "usart_log_task.h"

namespace task {

void test_task::operator()() {
  for (;;) {
    for (;;) {
      osDelay(1000);
      usart_log_task::instance().log("hello");
    }
  }
}

}  // namespace task