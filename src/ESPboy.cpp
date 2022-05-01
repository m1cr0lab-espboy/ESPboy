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

    _frame_count = _fps = 0;

    _initTFT();
    _initMCP4725();
    _initMCP23017();

    _dac.setVoltage(show_espboy_logo ? 0 : 4095, false);

    if (show_espboy_logo) {

        _drawLogo();
        fadeIn();  while (_fading.active) _fade(); delay(wait_ms);
        fadeOut(); while (_fading.active) _fade();
        tft.fillScreen(0);
        fadeIn();

    }
    
}

void ESPboy::begin(const uint8_t logo_width, const uint8_t logo_height, const uint16_t *logo_data, const uint16_t wait_ms) {

    _frame_count = _fps = 0;

    _initTFT();
    _initMCP4725();
    _initMCP23017();

    _dac.setVoltage(0, false);
    _drawLogo(logo_width, logo_height, logo_data);
    fadeIn();  while (_fading.active) _fade(); delay(wait_ms);
    fadeOut(); while (_fading.active) _fade();
    tft.fillScreen(0);
    fadeIn();

}

void ESPboy::update() {
    
    if (_fading.active) _fade();

    button.read(_buttons = ~(_mpx.readGPIOAB() & 0xff));

    pixel.update();
    
    _updateFPS();

}

void ESPboy::_initTFT() {

    tft.init();

}

void ESPboy::_initMCP4725() {

    _dac.begin(0x60);

}

void ESPboy::_initMCP23017() {

    _mpx.begin_I2C();

    // Buttons
    for (uint8_t i=0; i<8; ++i) _mpx.pinMode(i, INPUT_PULLUP);

    // NeoPixel LED
    pixel.begin(_mpx);
    
}

uint8_t ESPboy::buttons() const {

    return _buttons;

}

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

void ESPboy::_drawLogo() {

    _drawLogo(pgm_read_byte(&ESPBOY_LOGO_WIDTH), pgm_read_byte(&ESPBOY_LOGO_HEIGHT), ESPBOY_LOGO);

}

void ESPboy::_drawLogo(const uint8_t width, const uint8_t height, const uint16_t *data) {

    LGFX_Sprite logo(&tft);

    logo.createSprite(width, height);
    logo.pushImage(0, 0, width, height, data);
    logo.pushSprite((TFT_WIDTH - width) >> 1, (TFT_HEIGHT - height) >> 1);
    logo.deleteSprite();

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

void ESPboy::_fade() {

    const uint32_t now = micros();

    if (now - _fading.last_us > 9999) {

             if ( _fading.inc && _fading.level + 4 < _DAC_MAX) _fading.level += 4;
        else if (!_fading.inc && _fading.level - 4 > _DAC_MIN) _fading.level -= 4;
        else {
            _fading.active = false;
            _fading.level  = _fading.inc ? _DAC_MAX : _DAC_MIN;
        }


        switch (_fading.level) {

            case _DAC_MIN:
                _dac.setVoltage(0, false);
                break;

            case _DAC_MAX:
                _dac.setVoltage(4095, false);
                break;
            
            default:
                _dac.setVoltage(_fading.level, false);

        }

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