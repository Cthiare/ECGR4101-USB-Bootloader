# Lab 3 — USB Bootloader (NUCLEO-L496ZG-P)

**Demo video:** _TODO: YouTube (unlisted) link_

**Team:** Person 1 — _name_ · Person 2 — _name_ · Person 3 — _name_ · Person 4 — _name_

## Repository layout

```
Lab3_Bootloader/     CubeMX project, linked at 0x08000000 (64 KB)
Lab3_Application/    CubeMX blink app, linked at 0x08010000 (960 KB)
tools/firmware_sender.py   Python host that sends the .bin over USB CDC
```

## How it works

### Update-mode trigger
_TODO (Person 2)_ — The bootloader enumerates as a USB CDC virtual COM port and
waits for the `"BOOT"` magic followed by a 4-byte length.

### Receiving and writing firmware
_TODO (Persons 2 & 3)_ — erase of pages 32–511 across both banks, double-word
buffering, 0xFF padding of the final partial double word.

### Jumping to the application
_TODO (Person 1)_ — On a User-button press (ignored mid-transfer) the bootloader
checks the app's vector table, de-initializes USB/clocks/peripherals, disables
and clears all interrupts, sets `SCB->VTOR = 0x08010000`, loads the app's MSP and
branches to its `Reset_Handler`.

### Python sender
_TODO (Person 4)_

## Protocol

| Step | Direction | Bytes |
|---|---|---|
| 1 | PC → board | `"BOOT"` |
| 2 | PC → board | image length, `uint32_t` little-endian |
| 3 | board → PC | `'R'` after erase (~10 s) |
| 4 | PC → board | 256-byte chunks |
| 5 | board → PC | `'A'` after each chunk |
| 6 | board → PC | `'D'` done / `'E'` error |

## Build & flash

1. Build and flash **Lab3_Bootloader** with ST-LINK (normal flash at 0x08000000).
2. Build **Lab3_Application** with `FAST_BLINK 0` and flash its `.bin` at **0x08010000**:
   ```
   STM32_Programmer_CLI -c port=SWD -w Lab3_Application/build/Debug/Lab3_Application.bin 0x08010000 -v -rst
   ```
3. Set `FAST_BLINK 1`, rebuild, then plug into the USB OTG FS port and run:
   ```
   python tools/firmware_sender.py --port COM5 --bin Lab3_Application/build/Debug/Lab3_Application.bin
   ```
4. Press the User button → fast blink.

## Workflow

Each person works on their own branch and merges into `main` through a pull
request. `main` always holds the latest working version.
