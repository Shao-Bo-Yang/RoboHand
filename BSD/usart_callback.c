#include "main.h"
#include "noti_center.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart6) {
    BASE_NOTI_WITH_TYPE(BASE_NOTI_TYPE_USART6_WRITTEN);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart6) {
    BASE_NOTI_WITH_TYPE(BASE_NOTI_TYPE_USART6_WRITTEN);
  }
}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart6) {
    BASE_NOTI_WITH_TYPE(BASE_NOTI_TYPE_USART6_WRITTEN);
  }
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart6) {
    BASE_NOTI_WITH_TYPE(BASE_NOTI_TYPE_USART6_WRITTEN);
  }
}
