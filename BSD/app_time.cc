#include "app_time.h"

#include <cstdint>

#include "tim.h"
extern "C"
{
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
}

static uint64_t _app_time_ms = 0;

extern "C" void Robo_App_Timer_Update(void)
{
    _app_time_ms += 1;
}

namespace bsd
{

void app_time::init()
{
    HAL_TIM_Base_Start_IT(&htim4);
}

uint64_t app_time::app_time_ms()
{
    _critical c;
    return _app_time_ms;
}

} // namespace bsd
