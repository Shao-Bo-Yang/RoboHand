#pragma once

#include <cstdint>
#include <functional>

#include "cmsis_os2.h"
namespace bsd {

template <typename Device>
struct bsd_getter {
  bsd_getter() : _device_mutex(osMutexNew(NULL)) {}

  static auto use_device(std::function<void(Device &)> &&callable,
                         uint32_t timeout = osWaitForever) {
    auto &&self = _self();
    auto &&instance = _instance();
    auto state = osMutexAcquire(self._device_mutex, timeout);
    if (state == osOK) {
      auto &instance_ = *static_cast<Device *>(&instance);
      callable(instance_);
      osMutexRelease(self._device_mutex);
    }
    return state;
  }

  ~bsd_getter() { osMutexDelete(_device_mutex); }

 private:
  static auto &_instance() {
    static Device instance{};
    return instance;
  }

  static auto &_self() { return static_cast<bsd_getter &>(_instance()); }

 private:
  osMutexId_t _device_mutex;
};

}  // namespace bsd