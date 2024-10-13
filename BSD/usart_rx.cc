#include "usart_rx.h"

#include <FreeRTOS.h>
#include <task.h>

#include "usart_callback.h"
#include <stm32f427xx.h>
#include <stm32f4xx_ll_usart.h>

namespace bsd
{

usart_rx::usart_rx() : bsd_getter<usart_rx>()
{
}

void usart_rx::init()
{
    LL_USART_EnableDirectionRx(USART2);
    LL_USART_EnableIT_RXNE(USART2);
    LL_USART_EnableIT_ERROR(USART2);
}

bool usart_rx::_has_data()
{
    return !Robo_Usart2_Rx_Buffer_Is_Empty();
}

bool usart_rx::has_data()
{
    _critical c;
    return _has_data();
}

bool usart_rx::buffer_full()
{
    _critical c;
    return _buffer_full();
}

bool usart_rx::_buffer_full()
{
    return Robo_Usart2_Rx_Buffer_Is_Full();
}

char usart_rx::pop_buffer()
{
    _critical c;
    if (!_has_data())
    {
        return '\0';
    }
    auto cur = Robo_Usart2_Rx_Buffer_Pop();
    return cur;
}

char usart_rx::top_data()
{
    _critical c;
    if (!_has_data())
    {
        return '\0';
    }
    return Robo_Usart2_Rx_Buffer_Top();
}

void usart_rx::read(std::string &str)
{
    _critical c;
    while (_has_data())
    {
        str.push_back(pop_buffer());
    }
}

} // namespace bsd
