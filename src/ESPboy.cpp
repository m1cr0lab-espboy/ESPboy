/**
 * ----------------------------------------------------------------------------
 * @file   ESPboy.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  ESPboy primary controller
 * ----------------------------------------------------------------------------
 */

#include "ESPboy.h"

ESPboy espboy;

void ESPboy::begin(__FlashStringHelper const * const title, uint16 const color) {

    if (_initialized) return;

    uint8_t len = strlen_P((PGM_P)title);
    char    buffer[len + 1];
    strncpy_P(buffer, (PGM_P)title, len + 1);

    begin(buffer, color);

}

void ESPboy::begin(char const * const title, uint16 const color) {

    if (_initialized) return;

    _init(); if (_buttons) return;

    _showESPboyLogo(title, color);
    _fadeInOut(1000);

    tft.setTextColor(TFT_WHITE); // reset default color
    fadeIn();

    _initialized = true;

}

void ESPboy::begin(uint8_t const logo_width, uint8_t const logo_height, uint8_t const * const logo_bitmap, uint16_t const logo_color, uint16_t const wait_ms) {

    if (_initialized) return;

    _init(); if (_buttons) return;
    _showESPboyLogo();
    _fadeInOut(1000);

    tft.drawBitmap(
        (TFT_WIDTH  - logo_width)  >> 1,
        (TFT_HEIGHT - logo_height) >> 1,
        logo_bitmap,
        logo_width,
        logo_height,
        logo_color
    );

    _fadeInOut(wait_ms);

    tft.setTextColor(TFT_WHITE); // reset default color
    fadeIn();

    _initialized = true;

}

void ESPboy::begin(uint8_t const logo_width, uint8_t const logo_height, uint16_t const * const logo_colormap, uint16_t const wait_ms) {

    if (_initialized) return;

    _init(); if (_buttons) return;
    _showESPboyLogo();
    _fadeInOut(1000);

    tft.pushImage(
        (TFT_WIDTH  - logo_width)  >> 1,
        (TFT_HEIGHT - logo_height) >> 1,
        logo_width,
        logo_height,
        logo_colormap
    );

    _fadeInOut(wait_ms);

    tft.setTextColor(TFT_WHITE); // reset default color
    fadeIn();

    _initialized = true;

}

void ESPboy::_showESPboyLogo(char const * const title, uint16 const color) {

    dac.setVoltage(0, false);

    uint8_t padding = title == nullptr ? 0 : 16;
    uint8_t y = (TFT_HEIGHT - ESPBOY_LOGO_HEIGHT - 3 - 1 - 3 - 8 - 4 - 8 - 4 - TINY_M1CR0LAB_HEIGHT - padding - 8) >> 1;

    tft.drawBitmap(
        (TFT_WIDTH  - ESPBOY_LOGO_WIDTH) >> 1,
        y,
        ESPBOY_LOGO,
        ESPBOY_LOGO_WIDTH,
        ESPBOY_LOGO_HEIGHT,
        TFT_YELLOW
    );

    tft.drawFastHLine(42, y += ESPBOY_LOGO_HEIGHT + 3, 43, TFT_YELLOW);
    tft.setTextColor(TFT_YELLOW);
    tft.drawCenterString(F("ESPboy"), TFT_WIDTH >> 1, y += 1 + 3);
    tft.setTextColor(0xb596); // hsl(0, 0, 70)
    tft.drawCenterString(F("powered by"), TFT_WIDTH >> 1, y += 8 + 4);

    tft.drawBitmap(
        (TFT_WIDTH  - TINY_M1CR0LAB_WIDTH) >> 1,
        y += 8 + 4,
        TINY_M1CR0LAB_LOGO,
        TINY_M1CR0LAB_WIDTH,
        TINY_M1CR0LAB_HEIGHT,
        0x07f5 // hsl(160, 100, 50)
    );

    if (title != nullptr) {
        tft.setTextColor(color);
        tft.drawCenterString(title, TFT_WIDTH >> 1, y + TINY_M1CR0LAB_HEIGHT + padding);
    }

}

void ESPboy::_init() {
    
    _frame_count = _fps = 0;

    dac.begin(0x60);
    _initMCP23017();
    
    tft.init();
    tft.setBrightness(0xff);

    _buttons = ~(mcp.readGPIOAB() & 0xff);

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

void ESPboy::update() {
    
    if (_fading.active) _fade();

    button.read((_buttons = ~(mcp.readGPIOAB() & 0xff)));

    pixel.update();
    
    _updateFPS();

}

uint8_t ESPboy::buttons() const { return _buttons; }

// To please Roman 😉
uint8_t ESPboy::getKeys() { return ~(mcp.readGPIOAB() & 0xff); }

void ESPboy::_updateFPS() {

    static uint32_t last_sec = 0;
    uint32_t const sec = millis() / 1000;

    if (sec != last_sec) {
        _fps = _frame_count;
        _frame_count = 0;
        last_sec = sec;
    }

    _frame_count++;

}

uint32_t ESPboy::fps() const { return _fps; }

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

void ESPboy::dim(uint16_t const brightness) {

    switch (brightness) {

        case _DAC_MIN:
            dac.setVoltage(0, false);
            break;

        case _DAC_MAX:
            dac.setVoltage(4095, false);
            break;
        
        default:
            dac.setVoltage(brightness < 4095 ? brightness : 4095, false);

    }

}

void ESPboy::_fadeInOut(uint16_t const wait_ms) {

    fadeIn();  while (_fading.active) _fade(); delay(wait_ms);
    fadeOut(); while (_fading.active) _fade();
    tft.fillScreen(0);

}

void ESPboy::_fade() {

    uint32_t const now = micros();

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

/*
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021-2022 Stéphane Calderoni (https://github.com/m1cr0lab)
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