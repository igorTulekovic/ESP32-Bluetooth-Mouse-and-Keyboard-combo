# Contributing

Thanks for your interest. This is a small hobby project, so the process is simple.

## Reporting problems

Open an issue and include:
- Your ESP32 board and the ESP32 Arduino core version
- Versions of the libraries listed in the README
- What you expected, what happened, and the Serial Monitor output (115200 baud)
- Your wiring if it differs from [WIRING.md](WIRING.md)

## Suggesting or making changes

1. Fork the repository and create a branch for your change.
2. Keep changes focused: one fix or feature per pull request.
3. Test on real hardware and say what you tested in the pull request.
4. If you change a pin, a constant, or behaviour, update `README.md` and `WIRING.md` in the same pull request so the docs stay correct.
5. Open a pull request.

## Code style

- Follow the existing style: Arduino C++, 2-space indentation.
- Keep pin numbers and tunable values as `#define`s at the top of the `.ino`.
- Prefer passing the display and BLE objects by reference (`Adafruit_ST7789 &display`).
- Redraw only what changed on the display, to avoid flicker.

## Ideas that would help

- Implement keyboard mode
- A way to exit mouse mode back to the menu
- Redraw only the changed menu rows
- Scroll wheel support (ESP32BLECombo has `mouseScroll`)
- Use ADC1 pins for the joystick

## License

By contributing, you agree that your contributions are licensed under the MIT License.
