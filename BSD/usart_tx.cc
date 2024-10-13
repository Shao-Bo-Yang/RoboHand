#include "usart_tx.h"

#include <string>

#include "bsd_getter.h"
#include "usart_callback.h"
#include <stm32f427xx.h>
#include <stm32f4xx_ll_usart.h>

namespace bsd
{

usart_tx::usart_tx() : bsd_getter(), _tx_buf()
{
}

void usart_tx::init()
{
    LL_USART_EnableDirectionTx(USART2);
}

void usart_tx::write(const std::string &str)
{
    _critical c;
    _tx_buf = str;
    for (auto c : _tx_buf)
    {
        Robo_Usart2_Tx_Buffer_Push(c);
    }
    Robo_Usart2_Tx_Buffer_Push('\0');
    if (!LL_USART_IsEnabledIT_TXE(USART2))
    {
        LL_USART_EnableIT_TXE(USART2);
    }
}

} // namespace bsd
