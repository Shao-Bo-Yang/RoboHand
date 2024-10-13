#pragma once

#include <FreeRTOS.h>
#include <task.h>

#include <string>

#include "bsd_getter.h"
namespace bsd
{

struct usart_rx : public bsd_getter<usart_rx>
{
  public:
    void init();

  public:
    bool has_data();

    bool buffer_full();

    char pop_buffer();

    char top_data();

    void read(std::string &str);

  private:
    bool _has_data();
    bool _buffer_full();

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

    usart_rx();
    ~usart_rx() = default;
    usart_rx(const usart_rx &) = delete;
    usart_rx &operator=(const usart_rx &) = delete;

    friend bsd_getter;
};

} // namespace bsd
