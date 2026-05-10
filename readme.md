# pibookintosh

*Keyboard driver for classic 1999 iBook keyboard on RPi Pico*

* Keyboard Maintainer: [Gregory Smith](https://github.com/devcybiko)
* Hardware Supported: Raspberry Pi Pico with iBook keyboard matrix
* Hardware Availability: Custom handwired prototype

Make example for this keyboard (after setting up your build environment):

    make pibookintosh:default

Flashing example for this keyboard:

    make pibookintosh:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available

## Building

* This repo is only the driver. You should `git clone` it and then in the QMK 
* `ln -s ~/git/pibookintosh-driver ./keyboards/pibookintosh`
* Then in the QMK home folder do 
* `make pibookintosh:default`

## iBook keyboard

* I did a lot of initial work on the keyboard deconstruction and implementation
  * (Dr Francintosh: https://youtu.be/arHyYId8M54?si=swXaMM9lI4FwCPXM)
  * (https://www.youtube.com/watch?v=RTWwxyiINPI)

* However, it was Aaron Eiche's work that documented it best. He borrowed my notes, implemented his version, and created the matrix mapping. (Attached images are thanks to Aaron Eiche). Thanks Aaron!
    * https://aaroneiche.com/projects/pibook/
    * [connector_schematic](./images/connector_schematic.png)
    * [connector_image](./images/connector_image.png)
    * [keyboard_matrix](./images/keyboard_matrix.png)


## MCP23017

* The iBook keyboard uses 10 rows and 8 columns. But it also uses 7 additional lines for meta keys (shift, option, control, etc...), and 2 more for caps lock and power leds.
* This amounts to 27 GPIO pins which is nearly all the poor RPi Pico has to offer.
* So I used the MCP23017 I/O extender with I2C to add 16 GPIO lines
* (Aaron Eiche used shift registers)
* The MCP23017-specific code is isolated to its own mcp23017.c file

## File Organization

* qmk_callbacks.c - all the QMK callbacks / entrypoints I implemented are here. QMK has been surprisingly easy to work with. Still I personally can't easily tell QMK calls from others. So I isolated them to their own file. Basically, they call the pibookintosh_* functions.
* mcp23017.c - all the MCP23017 I/O Extender code is here
* pibookintosh.c - the pibookintosh implementation
* config.h - PICO config
* keyboard.json - the matrix formed in a day
* keymaps/default/keymap.c - the lifeforms grew later at a substantially accelerated rate.
