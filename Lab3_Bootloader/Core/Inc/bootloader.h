/**
 * bootloader.h - USB update-mode state machine (Requirement 1)
 * Owner: Person 2
 *
 * Protocol (see README):
 *   PC -> "BOOT" | uint32 length (little-endian)
 *   board -> 'R' after erase
 *   PC -> 256-byte chunks, board -> 'A' after each
 *   board -> 'D' when done, 'E' on error
 */
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdbool.h>
#include <stdint.h>

/* Call once after MX_USB_DEVICE_Init(). */
void Bootloader_Init(void);

/* Call every pass of the main loop. Does all flash work (never in the USB IRQ). */
void Bootloader_Process(void);

/* Call from CDC_Receive_FS() in usbd_cdc_if.c. Only copies into a ring buffer. */
void Bootloader_RxBytes(const uint8_t *data, uint32_t len);

/* True while a transfer is in progress - the button jump is ignored then,
 * so we never launch a half-written application. */
bool Bootloader_IsBusy(void);

#endif /* BOOTLOADER_H */
