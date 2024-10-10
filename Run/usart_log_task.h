#pragma once

#include <list>
#include <optional>
#include <string>

#include "cmsis_os2.h"
#include "robo_task.h"
namespace task {

struct usart_log_task : public task_generic<usart_log_task> {
 public:
  void addition_init();

 public:
  void operator()();

  void log(std::string str);

 private:
  void _write(std::string &&);
  usart_log_task();
  ~usart_log_task() = default;

 private:
  std::optional<std::string> _tx_buf{};
  std::list<std::string> _buf{};
  osMutexId_t _mutex;
  osSemaphoreId_t _empty;
  osSemaphoreId_t _full;

  friend task_generic;
};

}  // namespace task