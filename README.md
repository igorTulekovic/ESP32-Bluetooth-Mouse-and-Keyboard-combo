# ESP32 Bluetooth Mouse and Keyboard Combo

An ESP32 that shows up on your PC as a Bluetooth (BLE) mouse and keyboard. You steer it with an analog joystick and two buttons, and a small ST7789 display shows a menu and the current mouse state.

> **Status: work in progress.** Mouse mode works. Keyboard mode is a stub (the menu entry exists, but it sends no keys yet). See [Known limitations](#known-limitations).

## Features

- Appears as a standard BLE HID keyboard + mouse (no drivers or software needed on the PC)
- Joystick-driven on-screen menu with a hover highlight and a red cursor dot
- Mouse mode: joystick moves the PC cursor, left and right buttons are left and right click
- Joystick centre is calibrated at every boot, so it works from USB or battery power
- Dead zone and proportional speed: the further you push the stick, the faster the cursor moves
- Mouse mode moves the PC cursor to the top-left corner on entry, so the on-screen cursor and the PC cursor start in sync
- Display shows a different 320x240 image for idle, left click, and right click

## Hardware

| Part | Notes |
|---|---|
| ESP32 dev board | Developed on an ESP32-D0WD-V3 (classic ESP32) |
| 2.0" ST7789 SPI display, 240x320 | GMT020-02-8P module, 8 pins |
| Analog joystick module | Two analog axes (the joystick's own push button is not used) |
| 2 push buttons | Left click and right click, each with a resistor (see [WIRING.md](WIRING.md)) |
| Breadboard and jumper wires | |
| Optional battery | See the power notes in [WIRING.md](WIRING.md) |

## Pinout

| Function | ESP32 pin |
|---|---|
| Display SCL / SCK | GPIO 18 (default SPI clock) |
| Display SDA / MOSI | GPIO 23 (default SPI data) |
| Display CS | GPIO 5 |
| Display DC | GPIO 2 |
| Display RST | GPIO 4 |
| Display VCC / BLK | 3.3V |
| Joystick X | GPIO 26 |
| Joystick Y | GPIO 27 |
| Left click button | GPIO 34 |
| Right click button | GPIO 35 |
| Second display CS (reserved, unused) | GPIO 25 |

Full wiring details and power notes are in [WIRING.md](WIRING.md).

## Software requirements

Install these in the Arduino IDE (Library Manager, or from GitHub for ESP32BLECombo):

| Library | Purpose |
|---|---|
| Adafruit GFX Library | Drawing |
| Adafruit ST7735 and ST7789 Library | Display driver |
| [ESP32BLECombo](https://github.com/kokofixcomputers/ESP32BLECombo) | BLE keyboard + mouse |
| NimBLE-Arduino | Required by ESP32BLECombo |

You also need the **ESP32 board package** for the Arduino IDE. The sketch has been built and uploaded with ESP32 core 3.3.11.

## Build and upload

1. Clone or download this repository and open `ESP32BluetoothMouseAndKeyboardCombo.ino` in the Arduino IDE. Keep all files (`menu.h`, `joystickAndButtonInput.h`, `icons.h`) in the same folder as the `.ino`.
2. Select your ESP32 board and COM port.
3. Upload. If the upload fails, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

## First use

1. Power the board. **Keep your hands off the joystick while the logo is shown.** The joystick centre is calibrated during this time (200 readings per axis).
2. On your PC, open Bluetooth settings and pair with **Sokol Universal Interface Hub** (the default device name, set in the `.ino`).
3. Use the joystick to move the red cursor dot over a menu item, then press the **left button** to select it.

If you change the device name or the HID mode, remove the old pairing on the PC and pair again.

## Using the menu

| Menu item | What it does |
|---|---|
| Keyboard | Not implemented yet |
| Mouse | Switches to mouse mode |

**Mouse mode:**
- After about 1.5 seconds, the PC cursor is pushed to the top-left corner of the desktop.
- Joystick moves the PC cursor. Left and right buttons act as left and right click.
- The display shows the idle image, or the left-click or right-click image while a button is held.

## Configuration

These constants are at the top of `ESP32BluetoothMouseAndKeyboardCombo.ino`:

| Constant | Default | Meaning |
|---|---|---|
| `PIXELS_PER_MOVEMENT` | 10 | Maximum cursor movement per update at full joystick push (Y uses 75% of this) |
| `JOYSTICK_DEADBAND` | 200 | Raw ADC units around the centre with no movement |
| `JOYSTICK_HALF_RANGE` | 1900 | Distance from centre to edge of the 12-bit ADC range |
| `TFT_CS`, `TFT_DC`, `TFT_RST` | 5, 2, 4 | Display pins |
| `JOYSTICK_X`, `JOYSTICK_Y` | 26, 27 | Joystick pins |
| `LEFT_MOUSE_CLICK`, `RIGHT_MOUSE_CLICK` | 34, 35 | Button pins (active HIGH) |

The Bluetooth name, manufacturer, and battery level are set in the `ESP32BLEComboConfig` block in `setup()`.

## How it works

1. **Calibration:** at boot, the sketch averages 200 joystick readings per axis to find the resting centre.
2. **Joystick to speed:** each axis is averaged over 16 readings, the dead band is removed, and the result is scaled to -1.0 .. +1.0, then multiplied by `PIXELS_PER_MOVEMENT`.
3. **Cursor position:** the sketch keeps its own cursor position (0..320 x 0..240 in the menu, 0..1920 x 0..1080 in mouse mode).
4. **Mouse output:** in mouse mode, the change in cursor position is sent to the PC as relative mouse movement through ESP32BLECombo. A Bluetooth mouse can only send relative movement, which is why the cursor is homed to the top-left corner first.

## Project structure

| File | Contents |
|---|---|
| `ESP32BluetoothMouseAndKeyboardCombo.ino` | Setup, joystick maths, menu and mouse mode logic |
| `menu.h` | Menu items, display drawing, hover detection, mouse-mode screen |
| `joystickAndButtonInput.h` | Sends mouse movement and clicks over Bluetooth; keyboard stub |
| `icons.h` | 1-bit bitmaps: logo, mouse idle, mouse left click, mouse right click |
| `WIRING.md` | Wiring and power notes |
| `TROUBLESHOOTING.md` | Fixes for problems seen so far |
| `CONTRIBUTING.md` | How to contribute |

## Known limitations

- **Keyboard mode is not implemented.** `keyboardControl()` and `keyboardOption()` are empty.
- **No way back to the menu from mouse mode.** As far as the code shows, once Mouse is selected, the left button keeps mouse mode active, so you need to reset or power-cycle the board to return to the menu.
- **Screen size is assumed.** Mouse mode uses a fixed 1920x1080 area, so other resolutions and multi-monitor layouts may behave differently.
- **The tracked cursor can drift.** The sketch estimates where the PC cursor is. Operating system pointer acceleration can make the estimate differ from the real position over time.
- **The menu flickers while the cursor moves**, because the whole screen is redrawn on every cursor change.
- **Joystick on ADC2 pins.** GPIO 26 and 27 are ADC2 pins, which stop working if Wi-Fi is enabled. Don't use Wi-Fi in this sketch, or move the joystick to ADC1 pins (GPIO 32-39).

## Troubleshooting

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT. See [LICENSE](LICENSE).
