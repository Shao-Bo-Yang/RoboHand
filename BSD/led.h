#pragma once

#include <cstdint>
#include <utility>

#include "bsd_getter.h"
extern "C"
{
#include "main.h"

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
}

namespace bsd
{

enum class signal_led_type
{
    Work,
    Err,
};

namespace
{

template <typename led_cls, led_cls led_id> struct _led
{
};

template <> struct _led<uint32_t, 1>
{
    static auto pin()
    {
        return std::make_pair(LED1_GPIO_Port, LED1_Pin);
    }
};

template <> struct _led<uint32_t, 2>
{
    static auto pin()
    {
        return std::make_pair(LED2_GPIO_Port, LED2_Pin);
    }
};

template <> struct _led<uint32_t, 3>
{
    static auto pin()
    {
        return std::make_pair(LED3_GPIO_Port, LED3_Pin);
    }
};

template <> struct _led<uint32_t, 4>
{
    static auto pin()
    {
        return std::make_pair(LED4_GPIO_Port, LED4_Pin);
    }
};

template <> struct _led<uint32_t, 5>
{
    static auto pin()
    {
        return std::make_pair(LED5_GPIO_Port, LED5_Pin);
    }
};

template <> struct _led<uint32_t, 6>
{
    static auto pin()
    {
        return std::make_pair(LED6_GPIO_Port, LED6_Pin);
    }
};

template <> struct _led<uint32_t, 7>
{
    static auto pin()
    {
        return std::make_pair(LED7_GPIO_Port, LED7_Pin);
    }
};

template <> struct _led<uint32_t, 8>
{
    static auto pin()
    {
        return std::make_pair(LED8_GPIO_Port, LED8_Pin);
    }
};

template <> struct _led<signal_led_type, signal_led_type::Work>
{
    static auto pin()
    {
        return std::make_pair(WORK_LED_GPIO_Port, WORK_LED_Pin);
    }
};

template <> struct _led<signal_led_type, signal_led_type::Err>
{
    static auto pin()
    {
        return std::make_pair(ERR_LED_GPIO_Port, ERR_LED_Pin);
    }
};

} // namespace

struct led : public bsd_getter<led>
{
  public:
    void init();

    template <uint8_t led_id, bool on> void switch_led()
    {
        auto [port, pin] = _led<uint32_t, led_id>::pin();
        if constexpr (on)
        {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        }
    }
    template <signal_led_type led_type, bool on> void switch_led()
    {
        auto [port, pin] = _led<signal_led_type, led_type>::pin();
        if constexpr (on)
        {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        }
    }

  private:
    led();
    virtual ~led() = default;
    led(const led &) = delete;
    led &operator=(const led &) = delete;

    friend bsd_getter;
};

} // namespace bsd
