# Wiring

All pin numbers are GPIO numbers on the ESP32. They match the `#define` lines at the top of `ESP32BluetoothMouseAndKeyboardCombo.ino`.

## Display (ST7789, GMT020-02-8P, 8 pins)

| Display pin | ESP32 pin | Notes |
|---|---|---|
| VCC | 3.3V | 3.3V only |
| GND | GND | |
| SCL / SCK | GPIO 18 | Default SPI clock |
| SDA / MOSI | GPIO 23 | Default SPI data. This is SPI data, not I2C |
| RES / RST | GPIO 4 | The sketch resets the display itself with a short pulse |
| DC | GPIO 2 | |
| CS | GPIO 5 | |
| BLK (backlight) | 3.3V | Or a GPIO if you want to dim it with PWM |

**Check the pin order on your module.** Labels and order differ between sellers, so go by the text printed on the board, not by this table's order.

## Joystick

| Joystick pin | ESP32 pin |
|---|---|
| VCC (+5V / VCC) | **3.3V** |
| GND | GND |
| VRx | GPIO 26 |
| VRy | GPIO 27 |
| SW (push button) | Not used |

Power the joystick from the **3.3V pin**, not from 5V or directly from a battery. The joystick outputs a fraction of whatever voltage it is powered with, and the ESP32's analog inputs should not see more than 3.3V.

## Buttons

| Button | ESP32 pin |
|---|---|
| Left click | GPIO 34 |
| Right click | GPIO 35 |

The sketch reads a pressed button as **HIGH**. Each button connects the pin to 3.3V when pressed, with a resistor from the pin to GND (a pull-down, typically around 10 kΩ). GPIO 34 and 35 are input-only and have no internal pull resistors, so the external resistor is required.

## Reserved and unused pins

| Pin | Status |
|---|---|
| GPIO 25 | `TFT_CS2`, reserved for an optional second display (not used in the current sketch) |
| GPIO 14 | `colorReadPin`, defined but unused in the current sketch |

## Wiring overview

```
              ESP32
          ┌───────────┐
 3.3V ────┤ 3V3       │
 GND  ────┤ GND       │
          │           │
 Display  │ GPIO 18   ├── SCL
          │ GPIO 23   ├── SDA
          │ GPIO 5    ├── CS
          │ GPIO 2    ├── DC
          │ GPIO 4    ├── RES
          │           │
 Joystick │ GPIO 26   ├── VRx
          │ GPIO 27   ├── VRy
          │           │
 Buttons  │ GPIO 34   ├── Left click  (3.3V on press, resistor to GND)
          │ GPIO 35   ├── Right click (3.3V on press, resistor to GND)
          └───────────┘
```

## Power notes

- **USB:** works out of the box.
- **Battery:** the joystick centre is calibrated at every boot, so the cursor should stay still on battery power as long as you don't touch the stick while the logo is showing. Feed the board through its regulated input (its VIN or 5V pin, with a voltage in the range your board accepts) or through a proper regulator. Do not connect a raw lithium cell directly to the 3V3 pin: a fully charged cell is above the ESP32's 3.6V limit.
- **Noise:** if the cursor still creeps slowly on battery, a 0.1 µF ceramic capacitor from each joystick output pin (VRx, VRy) to GND, close to the ESP32, can help.

## Pin warnings

- **GPIO 2 and 5 are boot-strapping pins.** They normally work as outputs. If uploads fail or the board won't start with the display connected, unplug the display wires while flashing.
- **GPIO 6-11** are connected to the on-board flash and must never be used.
- **GPIO 26 and 27 are ADC2.** ADC2 does not work while Wi-Fi is enabled.
