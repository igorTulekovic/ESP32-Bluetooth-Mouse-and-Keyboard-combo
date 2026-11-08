# Troubleshooting

Fixes for problems seen while building this project. The first two sections are the most common ones.

## Upload fails

**"MD5 of file does not match data in flash" or "Failed to communicate with the flash chip"**

1. Disconnect the display, joystick, and buttons, leaving only the USB cable.
2. Try a different USB cable (a real data cable) and a different USB port, plugged straight into the PC.
3. In the Arduino IDE, set **Tools → Upload Speed** to 115200.
4. Enable **Tools → Erase All Flash Before Sketch Upload**.
5. Hold the BOOT button when "Connecting...." appears, and release it when the write starts.
6. If it still fails with nothing connected, the board itself may be faulty. Try a different ESP32.

**"A serial exception error occurred ... PermissionError"**

The PC sees the COM port but the board's USB-serial chip is not responding. Unplug and replug the board, close the Serial Monitor and any other program using the port, check the port in Device Manager for a warning icon, and install the driver for your board's USB chip (CP210x or CH340).

## Board reboots in a loop

**`E (31) qio_mode: Failed to set QIE bit, not enabling QIO mode` followed by `assert failed ... init_flash`**

The flash chip rejects QIO mode. In the Arduino IDE, set **Tools → Flash Mode → DIO** and **Flash Frequency → 40MHz**, erase the flash, and upload again. If the error persists, the flash chip or the board may be faulty.

**`Guru Meditation Error: LoadProhibited`**

An invalid memory read in your code. Common causes are a loop that runs past the end of an array (for example using `sizeof(items)` instead of the item count) or passing the display object by value instead of by reference. Decode the backtrace with the ESP Exception Decoder to find the line.

## Cursor moves by itself

- **Hands on the stick during boot.** The centre is calibrated while the logo is shown. Reset the board and leave the stick alone until the menu appears.
- **Slow creep on battery power.** Raise `JOYSTICK_DEADBAND` (try 200-250), power the joystick from the 3.3V pin, and add 0.1 µF capacitors from VRx and VRy to GND.
- **Cursor goes the wrong way.** Flip the sign on the affected axis in `universalJoystickMovement()`.
- **Full speed reached too early or never.** Adjust `JOYSTICK_HALF_RANGE`. Print the raw readings at each extreme of the stick and use a value slightly below the shortest centre-to-edge distance.

## Screen problems

- **Blank or white screen:** check the backlight (BLK) connection, then the RES, DC, and CS wires, and confirm the pin order against your module's silkscreen.
- **Wrong colours:** try inverting the display or swapping the colour order (`tft.invertDisplay(true)`).
- **Flickering menu:** known issue. The whole screen is redrawn whenever the cursor moves. Redrawing only the changed rows would fix it.

## PC cursor ends up in the wrong place

- On entering mouse mode, the board pushes the PC cursor into the top-left corner. With multiple monitors, the corner of the whole desktop may be somewhere unexpected.
- Operating system pointer acceleration makes the PC cursor travel farther than the board estimates, so the two positions can drift apart. Re-enter mouse mode (reset the board) to re-home.
- Mouse mode assumes a 1920x1080 area.

## Bluetooth problems

- **Device not found:** make sure the board is powered and the sketch is running, then scan again.
- **Paired but nothing happens, or it stopped working after a code change:** remove the device from the PC's Bluetooth list and pair again. The host caches the HID description, and re-pairing is required after the device mode or name changes.
- **Typing drops characters (once keyboard mode exists):** ESP32BLECombo has settings to slow key sending (`setKeyPressDelay`, `setKeyReleaseDelay`, `setKeyIntervalDelay`).
