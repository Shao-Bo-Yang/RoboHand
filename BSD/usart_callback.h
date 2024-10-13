#pragma once

#include <cstdint>
#ifdef __cplusplus
extern "C"
{
#endif

    extern const uint32_t Robo_Usart2_Rx_Buffer_Size;
    extern char Robo_Usart2_Rx_Buffer[];

    extern const uint32_t Robo_Usart2_Tx_Buffer_Size;
    extern char Robo_Usart2_Tx_Buffer[];

    bool Robo_Usart2_Tx_Buffer_Is_Empty(void);

    bool Robo_Usart2_Tx_Buffer_Is_Full(void);

    char Robo_Usart2_Tx_Buffer_Top(void);

    void Robo_Usart2_Tx_Buffer_Push(char c);

    char Robo_Usart2_Tx_Buffer_Pop(void);

    bool Robo_Usart2_Rx_Buffer_Is_Empty(void);

    bool Robo_Usart2_Rx_Buffer_Is_Full(void);

    char Robo_Usart2_Rx_Buffer_Top(void);

    void Robo_Usart2_Rx_Buffer_Push(char c);

    char Robo_Usart2_Rx_Buffer_Pop(void);

#ifdef __cplusplus
}
#endif
