/**
 * bootloader.c - STUB. Person 2 replaces this file on the `usb-receive` branch.
 * Exists so main.c builds and Person 1 can test the button jump right away.
 */
#include "bootloader.h"

void Bootloader_Init(void)
{
    /* TODO (Person 2): reset ring buffer and state machine to IDLE */
}

void Bootloader_Process(void)
{
    /* TODO (Person 2): IDLE -> GOT_MAGIC -> GOT_LENGTH -> ERASING -> RECEIVING -> DONE */
}

void Bootloader_RxBytes(const uint8_t *data, uint32_t len)
{
    (void)data;
    (void)len;
    /* TODO (Person 2): push bytes into ring buffer */
}

bool Bootloader_IsBusy(void)
{
    return false;
}
