#include "noti_center.h"

#include <cstdint>

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "noti_center.hh"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"

uint8_t BASE_REGISTER_WITH_TYPE(osThreadId_t thread_id, noti_type type) {
  return base::noti_center::instance().register_with_type(thread_id, type) ? 1
                                                                           : 0;
}

osThreadId_t BASE_THREAD_ID_WITH_TYPE(noti_type type) {
  return base::noti_center::instance().thread_id_with_type(type);
}

uint8_t BASE_NOTI_WITH_TYPE(noti_type type) {
  return base::noti_center::instance().noti_with_type(type) ? 1 : 0;
}

namespace base {

bool noti_center::register_with_type(osThreadId_t thread_id, noti_type type) {
  return true;
  auto result = _mapping.insert({type, thread_id});
  return result.second;
}

osThreadId_t noti_center::thread_id_with_type(noti_type type) {
  auto find = _mapping.find(type);
  if (find == _mapping.end()) {
    return nullptr;
  }
  return find->second;
}

bool noti_center::noti_with_type(noti_type type) {
  auto thread_id = thread_id_with_type(type);
  if (thread_id == nullptr) {
    return false;
  }
  BaseType_t result = pdFALSE;
  vTaskNotifyGiveFromISR((TaskHandle_t)thread_id, &result);
  return result == pdTRUE;
}

void noti_center::wait_for_type(osThreadId_t thread_id, noti_type type) {
  auto thread_id_in_map = thread_id_with_type(type);
  if (thread_id != thread_id_in_map) {
    return;
  }
  ulTaskNotifyTake(pdTRUE, 1000);
}

}  // namespace base