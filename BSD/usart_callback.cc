#include "usart_callback.h"

#include <cstdint>

#include <stm32f427xx.h>
#include <stm32f4xx_it.h>
#include <stm32f4xx_ll_usart.h>

extern "C" const uint32_t Robo_Usart2_Rx_Buffer_Size = 512;
char Robo_Usart2_Rx_Buffer[Robo_Usart2_Rx_Buffer_Size];
uint32_t Robo_Usart2_Rx_Buffer_Head = 0;
uint32_t Robo_Usart2_Rx_Buffer_Tail = 0;

extern "C" const uint32_t Robo_Usart2_Tx_Buffer_Size = 512;
char Robo_Usart2_Tx_Buffer[Robo_Usart2_Rx_Buffer_Size];
uint32_t Robo_Usart2_Tx_Buffer_Head = 0;
uint32_t Robo_Usart2_Tx_Buffer_Tail = 0;

bool Robo_Usart2_Tx_Buffer_Is_Empty(void)
{
    return Robo_Usart2_Tx_Buffer_Head == Robo_Usart2_Tx_Buffer_Tail;
}

bool Robo_Usart2_Tx_Buffer_Is_Full(void)
{
    return ((Robo_Usart2_Tx_Buffer_Head + 1) % Robo_Usart2_Tx_Buffer_Size) == Robo_Usart2_Tx_Buffer_Tail;
}

char Robo_Usart2_Tx_Buffer_Top(void)
{
    if (Robo_Usart2_Tx_Buffer_Is_Empty())
    {
        return '\0';
    }
    return Robo_Usart2_Tx_Buffer[Robo_Usart2_Tx_Buffer_Tail];
}

void Robo_Usart2_Tx_Buffer_Push(char c)
{
    if (Robo_Usart2_Tx_Buffer_Is_Full())
    {
        return;
    }
    Robo_Usart2_Tx_Buffer[Robo_Usart2_Tx_Buffer_Head++] = c;
    Robo_Usart2_Tx_Buffer_Head %= Robo_Usart2_Tx_Buffer_Size;
}

char Robo_Usart2_Tx_Buffer_Pop(void)
{
    if (Robo_Usart2_Tx_Buffer_Is_Empty())
    {
        return '\0';
    }
    auto result = Robo_Usart2_Tx_Buffer[Robo_Usart2_Tx_Buffer_Tail++];
    Robo_Usart2_Tx_Buffer_Tail %= Robo_Usart2_Tx_Buffer_Size;
    return result;
}

bool Robo_Usart2_Rx_Buffer_Is_Empty(void)
{
    return Robo_Usart2_Rx_Buffer_Head == Robo_Usart2_Rx_Buffer_Tail;
}

bool Robo_Usart2_Rx_Buffer_Is_Full(void)
{
    return ((Robo_Usart2_Rx_Buffer_Head + 1) % Robo_Usart2_Rx_Buffer_Size) == Robo_Usart2_Rx_Buffer_Tail;
}

char Robo_Usart2_Rx_Buffer_Top(void)
{
    if (Robo_Usart2_Rx_Buffer_Is_Empty())
    {
        return '\0';
    }
    return Robo_Usart2_Rx_Buffer[Robo_Usart2_Rx_Buffer_Tail];
}

void Robo_Usart2_Rx_Buffer_Push(char c)
{
    if (Robo_Usart2_Rx_Buffer_Is_Full())
    {
        return;
    }
    Robo_Usart2_Rx_Buffer[Robo_Usart2_Rx_Buffer_Head++] = c;
    Robo_Usart2_Rx_Buffer_Head %= Robo_Usart2_Rx_Buffer_Size;
}

char Robo_Usart2_Rx_Buffer_Pop(void)
{
    if (Robo_Usart2_Rx_Buffer_Is_Empty())
    {
        return '\0';
    }
    auto result = Robo_Usart2_Rx_Buffer[Robo_Usart2_Rx_Buffer_Tail++];
    Robo_Usart2_Rx_Buffer_Tail %= Robo_Usart2_Rx_Buffer_Size;
    return result;
}

extern "C" void Robo_Usart2_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(USART2))
    {
        auto data = LL_USART_ReceiveData8(USART2);
        if (Robo_Usart2_Rx_Buffer_Is_Full())
        {
            Robo_Usart2_Rx_Buffer_Pop();
        }
        Robo_Usart2_Rx_Buffer_Push(data);
        LL_USART_ClearFlag_RXNE(USART2);
    }
    if (LL_USART_IsActiveFlag_TXE(USART2))
    {
        if (Robo_Usart2_Tx_Buffer_Is_Empty())
        {
            LL_USART_DisableIT_TXE(USART2);
        }
        else
        {
            auto data = Robo_Usart2_Tx_Buffer_Pop();
            LL_USART_TransmitData8(USART2, data);
        }
    }
    if (LL_USART_IsActiveFlag_ORE(USART2))
    {
        LL_USART_ClearFlag_ORE(USART2);
    }
    if (LL_USART_IsActiveFlag_FE(USART2))
    {
        LL_USART_ClearFlag_FE(USART2);
    }
}
