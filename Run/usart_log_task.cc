#include "usart_log_task.h"

#include <cstdio>
#include <sstream>
#include <string>
#include <utility>

#include "debug_printf.h"
#include "usart_tx.h"
#include "work_led_task.h"
#include <cmsis_os2.h>

extern "C"
{

    void print_to_serial(const char *str)
    {
        task::usart_log_task::instance().print(str);
    }

    void log_to_serial(const char *str)
    {
        task::usart_log_task::instance().log(str);
    }
}

namespace task
{

usart_log_task::usart_log_task()
{
    _mutex = osMutexNew(nullptr);
    _empty = osSemaphoreNew(12, 0, nullptr);
    _full = osSemaphoreNew(12, 12, nullptr);
}

void usart_log_task::_write(std::string &&str)
{
    _tx_buf = std::move(str);
    bsd::usart_tx::use_device([&](bsd::usart_tx &usart) {
        auto &tx_buf = *_tx_buf;
        usart.write(tx_buf);
    });
    _tx_buf = nullptr;
}

void usart_log_task::addition_init()
{
}

void usart_log_task::log(const std::string &str)
{
    osSemaphoreAcquire(_full, osWaitForever);
    osMutexAcquire(_mutex, osWaitForever);
    std::stringstream ss;
    ss << "[LOG][" << osKernelGetTickCount() << "]:  " << str;
    _buf.emplace_back(ss.str());
    osMutexRelease(_mutex);
    osSemaphoreRelease(_empty);
}

void usart_log_task::print(const std::string &str)
{
    osSemaphoreAcquire(_full, osWaitForever);
    osMutexAcquire(_mutex, osWaitForever);
    std::stringstream ss;
    ss << "[LOG][" << osKernelGetTickCount() << "]:  " << str;
    _buf.emplace_back(ss.str());
    osMutexRelease(_mutex);
    osSemaphoreRelease(_empty);
}

void usart_log_task::operator()()
{
    for (;;)
    {
        osSemaphoreAcquire(_empty, osWaitForever);
        osMutexAcquire(_mutex, osWaitForever);
        std::string str = std::move(_buf.front());
        _buf.pop_front();
        osMutexRelease(_mutex);
        osSemaphoreRelease(_full);
        work_led_task::instance().blink();
        _write(std::move(str));
    }
}

} // namespace task
