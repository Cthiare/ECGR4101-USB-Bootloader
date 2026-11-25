/**
 * boot_jump.c - Hand-off from the bootloader to the application (Requirement 4)
 * Owner: Person 1
 *
 * Mirrors what the hardware does at reset (see "What Happens on Startup?"):
 *   word 0 of the vector table -> initial MSP
 *   word 1 of the vector table -> Reset_Handler address
 * The only difference is that we read the APPLICATION's table at 0x08010000
 * instead of the bootloader's at 0x08000000.
 */
#include "boot_jump.h"
#include "main.h"
#include "usbd_core.h"

extern USBD_HandleTypeDef hUsbDeviceFS;   /* defined in usb_device.c by CubeMX */

typedef void (*app_entry_t)(void);

bool Boot_AppIsValid(void)
{
    uint32_t app_msp   = *(volatile uint32_t *)(APP_START_ADDR);
    uint32_t app_reset = *(volatile uint32_t *)(APP_START_ADDR + 4U);

    /* Stack pointer must point into SRAM (top of stack may equal RAM_END). */
    if (app_msp < RAM_START_ADDR || app_msp > RAM_END_ADDR) {
        return false;
    }
    /* Reset handler must live inside the application's flash region. */
    if (app_reset < APP_START_ADDR || app_reset >= APP_END_ADDR) {
        return false;
    }
    return true;
}

bool Boot_ButtonPressed(void)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) {
        return false;
    }
    HAL_Delay(20);                                   /* simple debounce */
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_SET) {
        return false;
    }
    /* Wait for release so one press = one action. */
    while (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) { }
    return true;
}

void Boot_JumpToApp(void)
{
    if (!Boot_AppIsValid()) {
        return;                                      /* nothing to launch */
    }

    uint32_t    app_msp   = *(volatile uint32_t *)(APP_START_ADDR);
    app_entry_t app_reset = (app_entry_t)(*(volatile uint32_t *)(APP_START_ADDR + 4U));

    /* 1. Stop USB so the host sees a clean disconnect and no USB IRQs fire later. */
    USBD_Stop(&hUsbDeviceFS);
    USBD_DeInit(&hUsbDeviceFS);

    /* 2. Put clocks back to reset state (MSI 4 MHz). The blink app's
     *    SystemClock_Config() is empty and its SystemCoreClock starts at 4 MHz,
     *    so if we left the PLL running, HAL_Delay() in the app would be wrong. */
    HAL_RCC_DeInit();

    /* 3. Reset all peripherals (GPIO, etc.) back to defaults. */
    HAL_DeInit();

    /* 4. Stop SysTick (HAL_RCC_DeInit re-arms it). */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* 5. Disable every interrupt and clear anything pending. */
    __disable_irq();
    for (uint32_t i = 0; i < (sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0])); i++) {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    /* 6. Point the vector table at the application ("Relocating the Interrupt
     *    Vector Table"). Barriers make sure the write lands before we continue. */
    SCB->VTOR = APP_START_ADDR;
    __DSB();
    __ISB();

    /* 7. Load the application's initial stack pointer. */
    __set_MSP(app_msp);

    /* 8. Re-enable interrupts: after a real reset PRIMASK is 0, and the app's
     *    HAL_Delay() needs the SysTick exception. Nothing is enabled in the NVIC
     *    and SysTick is off, so nothing can fire before the app sets itself up. */
    __enable_irq();

    /* 9. Branch to the application's Reset_Handler. Never returns. */
    app_reset();

    while (1) { }
}
