#include "switch.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

namespace bsd
{

switch_on::switch_on() : bsd_getter<switch_on>()
{
}

void switch_on::init()
{
}

void switch_on::on()
{
    HAL_GPIO_WritePin(SWITCH_GPIO_Port, SWITCH_Pin, GPIO_PIN_SET);
}

void switch_on::off()
{
    HAL_GPIO_WritePin(SWITCH_GPIO_Port, SWITCH_Pin, GPIO_PIN_RESET);
}

bool switch_on::is_on()
{

    return HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == GPIO_PIN_SET;
}

} // namespace bsd
