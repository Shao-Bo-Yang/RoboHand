
#include "flash.h"

#include <cstdint>
#include <cstring>

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_flash.h>
#include <stm32f4xx_hal_flash_ex.h>

extern "C"
{

    extern volatile uint32_t _start_of_user_data;

    static __attribute__((section(
        ".user_data"))) volatile bsd::flash_data_t _flash_user_data[128 * (1 << 10) / sizeof(bsd::flash_data_t)];

    extern volatile uint32_t _end_of_user_data;
}

namespace bsd
{

static uint32_t _loaded_flag = 0xa0a0a0a0;
void flash::init()
{
    if (_start_of_user_data != _loaded_flag)
    {
        HAL_FLASH_Unlock();
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_OPERR |
                               FLASH_FLAG_RDERR | FLASH_FLAG_EOP | FLASH_FLAG_WRPERR);
        FLASH_Erase_Sector(FLASH_SECTOR_23, FLASH_VOLTAGE_RANGE_3);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&_start_of_user_data, _loaded_flag);
        HAL_FLASH_Lock();
    }
}

void flash::read_flash(flash_data_t *data, uint32_t length)
{
    std::memcpy(data, (void *)&_flash_user_data[1], length * sizeof(decltype(*data)));
}

void flash::write_flash(const flash_data_t *data, uint32_t length)
{
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_OPERR |
                           FLASH_FLAG_RDERR | FLASH_FLAG_EOP | FLASH_FLAG_WRPERR);
    FLASH_Erase_Sector(FLASH_SECTOR_23, FLASH_VOLTAGE_RANGE_3);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&_start_of_user_data, _loaded_flag);
    for (uint32_t i = 0; i < length; ++i)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&_flash_user_data[1 + i], data[i]);
    }
    HAL_FLASH_Lock();
}

flash::flash() : bsd_getter<flash>()
{
}

} // namespace bsd
