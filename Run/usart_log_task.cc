#include "usart_log_task.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

#include "cmsis_os2.h"
#include "noti_center.h"
#include "noti_center.hh"
#include "usart6.h"

namespace task {

usart_log_task::usart_log_task() {
  _mutex = osMutexNew(nullptr);
  _empty = osSemaphoreNew(12, 0, nullptr);
  _full = osSemaphoreNew(12, 12, nullptr);
}

void usart_log_task::_write(std::string&& str) {
  _tx_buf = std::move(str);
  bsd::usart6::use_device([&](bsd::usart6& usart) {
    auto& tx_buf = *_tx_buf;
    usart.write(tx_buf);
  });
  base::noti_center::instance().wait_for_type(get_task_id(),
                                              BASE_NOTI_TYPE_USART6_WRITTEN);
  _tx_buf = nullptr;
}

void usart_log_task::addition_init() {
  base::noti_center::instance().register_with_type(
      get_task_id(), BASE_NOTI_TYPE_USART6_WRITTEN);
}

void usart_log_task::log(std::string str) {
  osSemaphoreAcquire(_full, osWaitForever);
  osMutexAcquire(_mutex, osWaitForever);
  _buf.emplace_back(std::move(str));
  osMutexRelease(_mutex);
  osSemaphoreRelease(_empty);
}

void usart_log_task::operator()() {
  for (;;) {
    osSemaphoreAcquire(_empty, osWaitForever);
    osMutexAcquire(_mutex, osWaitForever);
    std::string str = std::move(_buf.front());
    _buf.pop_front();
    osMutexRelease(_mutex);
    osSemaphoreRelease(_full);
    str = "[LOG][" + std::to_string(osKernelGetTickCount()) + "]:  " + str;
    _write(std::move(str));
  }
}

}  // namespace task