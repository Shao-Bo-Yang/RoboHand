#include "pwm.h"

#include "bsd_getter.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"

namespace bsd
{

pwm::pwm() : bsd_getter()
{
}

void pwm::init()
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

void pwm::set_duty(double duty, pwm_channel ch)
{
    switch (ch)
    {
    case pwm_channel::Ch1:
        set_duty<pwm_channel::Ch1>(duty);
        break;
    case pwm_channel::Ch2:
        set_duty<pwm_channel::Ch2>(duty);
        break;
    case pwm_channel::Ch3:
        set_duty<pwm_channel::Ch3>(duty);
        break;
    case pwm_channel::Ch4:
        set_duty<pwm_channel::Ch4>(duty);
        break;
    case pwm_channel::Ch5:
        set_duty<pwm_channel::Ch5>(duty);
        break;
    case pwm_channel::Ch6:
        set_duty<pwm_channel::Ch6>(duty);
        break;
    case pwm_channel::Ch7:
        set_duty<pwm_channel::Ch7>(duty);
        break;
    case pwm_channel::Ch8:
        set_duty<pwm_channel::Ch8>(duty);
        break;
    }
}

} // namespace bsd
