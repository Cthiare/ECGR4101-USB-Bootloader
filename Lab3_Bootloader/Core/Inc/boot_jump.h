/**
 * boot_jump.h - Hand-off from the bootloader to the application (Requirement 4)
 * Owner: Person 1
 */
#ifndef BOOT_JUMP_H
#define BOOT_JUMP_H

#include <stdbool.h>
#include <stdint.h>

#define APP_START_ADDR   0x08010000UL                 /* must match the app linker ORIGIN */
#define APP_END_ADDR     0x08100000UL                 /* end of 1 MB flash */

/* RAM range from the linker script: 320K starting at 0x20000000 */
#define RAM_START_ADDR   0x20000000UL
#define RAM_END_ADDR     (RAM_START_ADDR + 320UL * 1024UL)

/* True if the app region holds something that looks like a real vector table
 * (valid initial MSP and a reset handler inside the app region).
 * Erased flash (0xFFFFFFFF) fails this check. */
bool Boot_AppIsValid(void);

/* Debounced read of the User button (B1, PC13, active-high on Nucleo-144). */
bool Boot_ButtonPressed(void);

/* Shut down the bootloader's peripherals, relocate VTOR, load the app's MSP
 * and branch to its Reset_Handler. Returns ONLY if no valid app is installed. */
void Boot_JumpToApp(void);

#endif /* BOOT_JUMP_H */
