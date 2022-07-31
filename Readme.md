# ESPboy Library

<img src="https://espboy.m1cr0lab.com/assets/espboy2-312x552.png" width="156" height="276" align="right" alt="ESPboy2">

This library provides a driver for the [ESPboy][espboy] multi-gadget handheld created by [Roman Sokolov][roman]. It gives you easy control over all its peripherals (128x128 TFT screen, 8 push buttons, and *NeoPixel* RGB LED).

ESPboy library relies on the following dependencies:

- [Adafruit MCP23017 Arduino Library][mcp23017]
- [Adafruit MCP4725 Arduino Library][mcp4725]
- [LovyanGFX][lovyangfx]

I noticed that most of the developments around the ESPboy use the [TFT_eSPI][tftespi] library, but I prefer [LovyanGFX][lovyangfx], which has better optimizations and more easy configuration.

## Getting started

The bare minimum to start coding with ESPboy library:

```cpp
#include <ESPboy.h>

void setup() {

    espboy.begin();

}

void loop() {

    espboy.update();

}
```

Please [read the user guide][doc] for more details.

## Acknowledgments

I want to thank the authors of [Adafruit MCP23017][mcp23017], [Adafruit MCP4725][mcp4725], and [LovyanGFX][lovyangfx] libraries, without whom the ESPboy library would not be what it is.

In particular, I warmly thank [lovyan03][lovyan03], who kindly agreed to [add support for ESP8266 in LovyanGFX][lovyan8266] and, more specifically, for the ESPboy, to make our lives easier with his outstanding graphics display driver. You can thank him by starring :star: the [LovyanGFX][lovyangfx] library.

I also thank [tobozo][tobozo], who helped me to configure the display device when I decided to use the LovyanGFX library for the ESPboy.


[espboy]:     https://www.espboy.com/
[roman]:      https://github.com/ESPboy-edu
[mcp23017]:   https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
[mcp4725]:    https://github.com/adafruit/Adafruit_MCP4725
[lovyangfx]:  https://github.com/lovyan03/LovyanGFX
[lovyan03]:   https://github.com/lovyan03
[tftespi]:    https://github.com/Bodmer/TFT_eSPI
[lovyan8266]: https://github.com/lovyan03/LovyanGFX/issues/130
[tobozo]:     https://github.com/tobozo
[doc]:        https://m1cr0lab-espboy.github.io/ESPboy