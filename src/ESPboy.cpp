/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * ESPboy Primary Controller
 * ----------------------------------------------------------------------------
 */
#include "ESPboy.h"

ESPboy espboy;

void ESPboy::begin(const bool show_espboy_logo, const uint16_t wait_ms) {

    if (_initialized) return;

    _init();

    dac.setVoltage(show_espboy_logo ? 0 : 4095, false);

    if (show_espboy_logo) {

        // tft.pushImage(
        //     (TFT_WIDTH  - ESPBOY_LOGO_WIDTH)  >> 1,
        //     (TFT_HEIGHT - ESPBOY_LOGO_HEIGHT) >> 1,
        //     ESPBOY_LOGO_WIDTH,
        //     ESPBOY_LOGO_HEIGHT,
        //     ESPBOY_LOGO
        // );

        uint8_t h = (TFT_HEIGHT - ESPBOY_LOGO_HEIGHT - 3 - 1 - 3 - 8) >> 1;

        tft.drawBitmap(
            (TFT_WIDTH  - ESPBOY_LOGO_WIDTH) >> 1,
            h,
            ESPBOY_LOGO,
            ESPBOY_LOGO_WIDTH,
            ESPBOY_LOGO_HEIGHT,
            TFT_YELLOW
        );

        tft.drawFastHLine(42, h + ESPBOY_LOGO_HEIGHT + 3, 43, TFT_YELLOW);
        tft.setTextColor(TFT_YELLOW);
        tft.drawString(F("ESPboy"), 46, h + ESPBOY_LOGO_HEIGHT + 3 + 1 + 3);

        _fadeInOut(wait_ms);

    }

    _initialized = true;
    
}

void ESPboy::begin(const uint8_t logo_width, const uint8_t logo_height, const uint16_t *logo_data, const uint16_t wait_ms) {

    if (_initialized) return;

    _init();

    dac.setVoltage(0, false);

    tft.pushImage(
        (TFT_WIDTH  - logo_width)  >> 1,
        (TFT_HEIGHT - logo_height) >> 1,
        logo_width,
        logo_height,
        logo_data
    );

    _fadeInOut(wait_ms);

    _initialized = true;

}

void ESPboy::_init() {
    
    _frame_count = _fps = 0;

    _initMCP4725();
    _initMCP23017();
    _initTFT();

    _buttons = ~(mcp.readGPIOAB() & 0xff);

}

void ESPboy::_initMCP4725() {

    dac.begin(0x60);

}

void ESPboy::_initMCP23017() {

    mcp.begin_I2C();

    // Buttons
    for (uint8_t i=0; i<8; ++i) mcp.pinMode(i, INPUT_PULLUP);

    // TFT Screen
    mcp.pinMode(_MCP23017_TFT_CS_PIN, OUTPUT);
    mcp.digitalWrite(_MCP23017_TFT_CS_PIN, LOW);

    // NeoPixel LED
    pixel.begin(mcp);
    
}

void ESPboy::_initTFT() {

    tft.init();

}

void ESPboy::update() {
    
    if (_fading.active) _fade();

    button.read((_buttons = ~(mcp.readGPIOAB() & 0xff)));

    pixel.update();
    
    _updateFPS();

}

uint8_t ESPboy::buttons() const { return _buttons; }

// To please Roman
uint8_t ESPboy::getKeys() const { return _buttons; }

void ESPboy::_updateFPS() {

    static uint32_t last_sec = 0;
    const  uint32_t sec      = millis() / 1000;

    if (sec != last_sec) {
        _fps = _frame_count;
        _frame_count = 0;
        last_sec = sec;
    }

    _frame_count++;

}

uint32_t ESPboy::fps() const {

    return _fps;

}

void ESPboy::_fadeInOut(const uint16_t wait_ms) {

    fadeIn();  while (_fading.active) _fade(); delay(wait_ms);
    fadeOut(); while (_fading.active) _fade();
    tft.fillScreen(0);
    fadeIn();

}

bool ESPboy::fading() const { return _fading.active; }

void ESPboy::fadeIn() {

    _fading.last_us = micros();
    _fading.level   = _DAC_MIN;
    _fading.inc     = true;
    _fading.active  = true;

}

void ESPboy::fadeOut() {

    _fading.last_us = micros();
    _fading.level   = _DAC_MAX;
    _fading.inc     = false;
    _fading.active  = true;

}

void ESPboy::dim(uint16_t const level) {

    switch (level) {

        case _DAC_MIN:
            dac.setVoltage(0, false);
            break;

        case _DAC_MAX:
            dac.setVoltage(4095, false);
            break;
        
        default:
            dac.setVoltage(level < 4095 ? level : 4095, false);

    }

}

void ESPboy::_fade() {

    const uint32_t now = micros();

    if (now - _fading.last_us > 9999) {

             if ( _fading.inc && _fading.level + 4 < _DAC_MAX) _fading.level += 4;
        else if (!_fading.inc && _fading.level - 4 > _DAC_MIN) _fading.level -= 4;
        else {
            _fading.active = false;
            _fading.level  = _fading.inc ? _DAC_MAX : _DAC_MIN;
        }

        dim(_fading.level);

        _fading.last_us = now;

    }

}

/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 */