#pragma once

extern "C"
{
#include <FreeRTOS.h>
#include <task.h>
}

#include <string>

#include "bsd_getter.h"
namespace bsd
{

struct usart_tx : public bsd_getter<usart_tx>
{
  public:
    void init();

  public:
    void write(const std::string &str);

  private:
    std::string _tx_buf;

  private:
    struct _critical
    {
        _critical()
        {
            taskENTER_CRITICAL();
        }
        ~_critical()
        {
            taskEXIT_CRITICAL();
        }
    };

    usart_tx();
    ~usart_tx() = default;
    usart_tx(const usart_tx &) = delete;
    usart_tx &operator=(const usart_tx &) = delete;

    friend bsd_getter;
};

} // namespace bsd
