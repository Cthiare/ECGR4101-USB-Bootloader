/**
 * flash_if.c - STUB. Person 3 replaces this file on the `flash-driver` branch.
 * Exists so the bootloader builds and Person 2 can test the USB path first.
 */
#include "flash_if.h"

HAL_StatusTypeDef FlashIF_EraseApp(void)
{
    return HAL_OK;   /* TODO (Person 3): erase both banks of the app region */
}

HAL_StatusTypeDef FlashIF_Write(const uint8_t *data, uint32_t len)
{
    (void)data;
    (void)len;
    return HAL_OK;   /* TODO (Person 3): double-word buffering + HAL_FLASH_Program */
}

HAL_StatusTypeDef FlashIF_Finish(void)
{
    return HAL_OK;   /* TODO (Person 3): pad with 0xFF, write last double word, lock */
}
