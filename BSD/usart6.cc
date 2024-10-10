
#include "usart6.h"

#include <cstdint>
#include <string>

#include "bsd_getter.h"
#include "cmsis_os2.h"
#include "main.h"
#include "stm32f427xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"

namespace bsd {

usart6::usart6() : bsd_getter() {}

void usart6::init() {}

void usart6::write(std::string& str) {
  HAL_UART_Transmit_DMA(&huart6, reinterpret_cast<const uint8_t*>(str.c_str()),
                        str.length() + 1);
}

}  // namespace bsd