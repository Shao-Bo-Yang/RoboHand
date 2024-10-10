#pragma once

#include <algorithm>
#include <cstdint>
#include <utility>

#include "bsd_getter.h"
#include "cmsis_os2.h"
#include "rw_lock.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_ll_tim.h"
#include "tim.h"

namespace bsd {

enum class pwm_channel {
  Ch1,
  Ch2,
  Ch3,
  Ch4,
  Ch5,
  Ch6,
  Ch7,
  Ch8,
};

namespace {

template <pwm_channel ch>
struct _ch {};

template <>
struct _ch<pwm_channel::Ch1> {
  static auto tim_ch_pair() { return std::make_pair(&htim2, TIM_CHANNEL_1); };
};

template <>
struct _ch<pwm_channel::Ch2> {
  static auto tim_ch_pair() { return std::make_pair(&htim2, TIM_CHANNEL_2); };
};

template <>
struct _ch<pwm_channel::Ch3> {
  static auto tim_ch_pair() { return std::make_pair(&htim2, TIM_CHANNEL_3); };
};

template <>
struct _ch<pwm_channel::Ch4> {
  static auto tim_ch_pair() { return std::make_pair(&htim2, TIM_CHANNEL_4); };
};

template <>
struct _ch<pwm_channel::Ch5> {
  static auto tim_ch_pair() { return std::make_pair(&htim8, TIM_CHANNEL_1); };
};

template <>
struct _ch<pwm_channel::Ch6> {
  static auto tim_ch_pair() { return std::make_pair(&htim8, TIM_CHANNEL_2); };
};

template <>
struct _ch<pwm_channel::Ch7> {
  static auto tim_ch_pair() { return std::make_pair(&htim8, TIM_CHANNEL_3); };
};

template <>
struct _ch<pwm_channel::Ch8> {
  static auto tim_ch_pair() { return std::make_pair(&htim8, TIM_CHANNEL_4); };
};

}  // namespace

struct pwm : public bsd_getter<pwm> {
 public:
  void init();

 public:
  template <pwm_channel ch>
  void set_duty(double duty) {
    auto [tim, channel] = _ch<ch>::tim_ch_pair();
    auto arr = __HAL_TIM_GET_AUTORELOAD(tim);
    duty = std::min(std::max(duty, 0.0), 1.0);
    __HAL_TIM_SET_COMPARE(tim, channel, duty * arr);
  }

  template <pwm_channel ch>
  auto get_duty() {
    auto [tim, channel] = _ch<ch>::tim_ch_pair();
    auto arr = __HAL_TIM_GET_AUTORELOAD(tim);
    auto ccr = __HAL_TIM_GET_COMPARE(tim, channel);
    return arr != 0 ? static_cast<double>(ccr) / arr : 0;
  }

  template <pwm_channel ch>
  auto get_cnt() {
    auto [tim, channel] = _ch<ch>::tim_ch_pair();
    return __HAL_TIM_GET_COUNTER(tim);
  }

  template <pwm_channel ch>
  auto get_ccr() {
    auto [tim, channel] = _ch<ch>::tim_ch_pair();
    return __HAL_TIM_GET_COMPARE(tim, channel);
  }

  template <pwm_channel ch>
  auto is_enable() {
    auto [tim, channel] = _ch<ch>::tim_ch_pair();
    return LL_TIM_IsEnabledCounter(tim->Instance);
  }

 private:
  pwm();
  virtual ~pwm() = default;
  pwm(const pwm &) = delete;
  pwm &operator=(const pwm &) = delete;

 private:
  friend bsd_getter;
};

}  // namespace bsd
