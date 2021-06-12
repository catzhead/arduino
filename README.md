# Arduino projects


# moulin-v2


The main project is a tachometer for the wheel of a [watermill in France](https://ondreville-sur-essonne.fr/le-moulin-de-chatillon/).


Note: the project uses an Arduino Mega2560 because it needs more pins than are
available on the standard Arduino uno.

To build with arduino-cli

    arduino-cli core install arduino:avr@1.8.2
    arduino-cli lib install TaskScheduler
    arduino-cli lib install LiquidCrystal
    arduino-cli lib install ArduinoSTL
    arduino-cli lib install LedControl
    arduino-cli compile -b arduino:avr:uno --output-dir build/
    arduino-cli upload -b arduino:avr:uno -p /dev/ttyUSB0 -v

Note: user must be in uucp group


# tests


Small projects to test several components the tachometer: LCD screen, LED panel,
etc.
