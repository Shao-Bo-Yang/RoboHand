#include "pwm.h"

#include "bsd_getter.h"
#include "rw_lock.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"

namespace bsd {

pwm::pwm() :bsd_getter()  {}

void pwm::init() {
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

}  // namespace bsd
