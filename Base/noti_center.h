#pragma once

#include <stdint.h>

#include "cmsis_os2.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  BASE_NOTI_TYPE_USART6_WRITTEN,

} noti_type;

uint8_t BASE_REGISTER_WITH_TYPE(osThreadId_t thread_id, noti_type type);

osThreadId_t BASE_THREAD_ID_WITH_TYPE(noti_type type);

uint8_t BASE_NOTI_WITH_TYPE(noti_type type);

#ifdef __cplusplus
}
#endif
