/**
 * flash_if.h - Application flash erase/program interface (Requirement 2)
 * Owner: Person 3
 *
 * DO NOT change these signatures without telling the team - Person 2's
 * bootloader.c calls them.
 */
#ifndef FLASH_IF_H
#define FLASH_IF_H

#include "main.h"
#include <stdint.h>

/* Erase application region: Bank 1 pages 32-255 and Bank 2 pages 0-255.
 * Takes ~10 s. Leaves the write pointer at 0x08010000. */
HAL_StatusTypeDef FlashIF_EraseApp(void);

/* Append len bytes to the image. Buffers into 8-byte double words and
 * programs each one as soon as it is full. */
HAL_StatusTypeDef FlashIF_Write(const uint8_t *data, uint32_t len);

/* Pad any leftover (<8) bytes with 0xFF, program them, and lock flash. */
HAL_StatusTypeDef FlashIF_Finish(void);

#endif /* FLASH_IF_H */
