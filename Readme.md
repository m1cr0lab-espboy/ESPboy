# ESPboy Library

<img src="https://espboy.m1cr0lab.com/assets/espboy2-312x552.png" width="156" height="276" align="right" alt="ESPboy2">

This library provides a driver for the [ESPboy][espboy] handheld designed by Roman Sokolov.

It relies on the following dependencies:

- [Adafruit MCP23017 Arduino Library][mcp23017]
- [Adafruit MCP4725 Arduino Library][mcp4725]
- [LovyanGFX][lovyangfx]

I noticed that most of the developments around the ESPboy have embraced Bodmer's [TFT_eSPI][tftespi] library, but I prefer the [LovyanGFX][lovyangfx] library which has better optimizations. Its author, lovyan03, has kindly agreed to [add support for ESP8266][lovyan8266] to make our life with ESPboy easier. You can thank him by starring :star: his library.

## Getting started

[Please read the user guide][doc].


[espboy]:     https://www.espboy.com/
[mcp23017]:   https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
[mcp4725]:    https://github.com/adafruit/Adafruit_MCP4725
[lovyangfx]:  https://github.com/lovyan03/LovyanGFX
[tftespi]:    https://github.com/Bodmer/TFT_eSPI
[lovyan8266]: https://github.com/lovyan03/LovyanGFX/issues/130
[doc]:        https://m1cr0lab-espboy.github.io/ESPboy
