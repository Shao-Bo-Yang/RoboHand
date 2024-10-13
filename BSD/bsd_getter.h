#pragma once

#include <FreeRTOS.h>

#include <cstdint>
#include <functional>

extern "C"
{

#include <cmsis_gcc.h>
#include <cmsis_os2.h>
}
namespace bsd
{

template <typename Device> struct bsd_getter
{
    bsd_getter() : _device_sema(osSemaphoreNew(1, 1, NULL))
    {
    }

    static auto use_device(std::function<void(Device &)> &&callable, uint32_t timeout = osWaitForever)
    {
        auto &&self = _self();
        auto &&instance = _instance();
        if (__get_IPSR() != 0)
        {
            auto &instance_ = *static_cast<Device *>(&instance);
            callable(instance_);
            return osOK;
        }
        else
        {
            auto state = osSemaphoreAcquire(self._device_sema, timeout);
            if (state == osOK)
            {
                auto &instance_ = *static_cast<Device *>(&instance);
                callable(instance_);
                osSemaphoreRelease(self._device_sema);
            }
            return state;
        }
    }

    ~bsd_getter()
    {
        osSemaphoreDelete(_device_sema);
    }

  protected:
    static auto &_instance()
    {
        static Device instance{};
        return instance;
    }

    static auto &_self()
    {
        return static_cast<bsd_getter &>(_instance());
    }

  private:
    osSemaphoreId_t _device_sema;
};

} // namespace bsd
