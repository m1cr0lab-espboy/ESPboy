/**
 * ----------------------------------------------------------------------------
 * @file   NeoPixel.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  NeoPixel controller
 * ----------------------------------------------------------------------------
 */

#include "NeoPixel.h"

void NeoPixel::begin(Adafruit_MCP23X17 &mcp) {

    pinMode(_LED_PIN, OUTPUT);

    _mcp = &mcp;
    _mcp->pinMode(_MCP23017_LED_LOCK_PIN, OUTPUT);

    _fx = _FX::NONE;
    
    setBrightness(0x20);
    clear();

}

void NeoPixel::update() {

    switch (_fx) {

        case _FX::FLASH:   _flash();   break;
        case _FX::BREATHE: _breathe(); break;
        case _FX::RAINBOW: _rainbow(); break;
        default:;

    }
    
}

void NeoPixel::setBrightness(uint8_t const b) { _brightness = b + 1; }

void NeoPixel::clear() const { show(0); }
void NeoPixel::reset() { _fx = _FX::NONE; clear(); }

void NeoPixel::show(uint32_t color) const {

    _show(_rgb2grb(color));

}

void NeoPixel::flash(uint32_t const color, uint16_t const duration_ms, uint8_t const count, uint16_t const period_ms) {

    _fx             = _FX::FLASH;
    _fx_color       = color;
    _fx_start_ms    = millis();
    _fx_duration_ms = duration_ms;
    _fx_period_ms   = period_ms;
    _fx_count       = count;
    _fx_looping     = count == 0;
    _flashing       = true;

    show(color);
    
}

void NeoPixel::breathe(uint32_t const color, uint16_t const period_ms, uint8_t const count) {

    if (!period_ms) return;

    _fx             = _FX::BREATHE;
    _fx_period_ms   = period_ms;
    _fx_offset      = 0;
    _fx_color       = color;
    _fx_start_ms    = millis();
    _fx_count       = count;
    _fx_looping     = count == 0;

}

void NeoPixel::rainbow(uint16_t const period_ms, uint8_t const count) {

    if (!period_ms) return;

    _fx             = _FX::RAINBOW;
    _fx_period_ms   = period_ms;
    _fx_offset      = 0;
    _fx_start_ms    = millis();
    _fx_count       = count;
    _fx_looping     = count == 0;

    show(Color::hsv2rgb(_fx_offset));

}

uint32_t NeoPixel::_rgb2grb(uint32_t const rgb) const {

    return (((rgb >> 8) & 0xff) << 16) |
            ((rgb >> 16)        <<  8) |
             (rgb & 0xff);

}

uint8_t NeoPixel::_sine(uint8_t const i) const {

    switch (i >> 6) {

        case 0:  return  pgm_read_byte(_FAST_SINE + i);           // [  0 -  63]
        case 1:  return ~pgm_read_byte(_FAST_SINE + (~i & 0x3f)); // [ 64 - 127]
        case 2:  return ~pgm_read_byte(_FAST_SINE + ( i & 0x3f)); // [128 - 191]
        default: return  pgm_read_byte(_FAST_SINE + (~i & 0x3f)); // [192 - 255]

    }

}

void NeoPixel::_flash() {

    if (!_fx_looping && !_fx_count) { _fx = _FX::NONE; return; }

    if (millis() - _fx_start_ms < _fx_duration_ms) return;

    if (_flashing) { clear(); _flashing = false; _fx_count--; return; }

    if (millis() - _fx_start_ms < _fx_period_ms) return;

    _fx_start_ms += _fx_period_ms;
    _flashing = true;

    show(_fx_color);

}

void NeoPixel::_breathe() {

    if (!_fx_looping && !_fx_count) { reset(); return; }

    static uint8_t n = 0;

    uint16_t r = (millis() - _fx_start_ms) % _fx_period_ms;
    uint8_t  l = 256 * r / _fx_period_ms;

    switch (n) {
        case 0: if (l > _fx_offset) n = 1; break;
        case 1: if (l < _fx_offset) n = 2; break;
        default: n = 0; _fx_count--; if (!_fx_count) return;
    }

    uint32_t const color = _fx_color;
    uint8_t  const lumin = _sine(_fx_offset = l) + 1;

    if (lumin) {
        uint8_t c, *p = (uint8_t*)&color;
        for (uint8_t i=0; i<3; ++i) {
            c = *p;
            *p++ = (c * lumin) >> 8;
        }
    }

    show(color);

}

void NeoPixel::_rainbow() {

    if (!_fx_looping && !_fx_count) { reset(); return; }

    uint16_t r = (millis() - _fx_start_ms) % _fx_period_ms;
    uint16_t h = 360 * r / _fx_period_ms;

    if (h < _fx_offset) _fx_count--;

    show(Color::hsv2rgb(_fx_offset = h));

}

/**
 * @note Inspired by these references:
 * @see  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/esp8266.c
 * @see  https://github.com/ESPboy-edu/ESPboy_Classes/blob/main/ESPboy_LED/ESPboyLED.cpp
 */
void IRAM_ATTR NeoPixel::_show(uint32_t const color) const {

    static const uint32_t pin_mask = 1 << _LED_PIN;
    uint32_t t, c, start, mask;

    if (_brightness) {

        uint8_t *p = (uint8_t*)&color;
        for (uint8_t i=0; i<3; ++i) {
            c = *p;
            *p++ = (c * _brightness) >> 8;
        }

    }
    
    mask  = 0x800000;
    start = 0;

    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pin_mask);  // light on the onboard LED
    _mcp->digitalWrite(_MCP23017_LED_LOCK_PIN, HIGH); // and open the transistor lock

    os_intr_lock();

    for (uint8_t i = 0; i < 24; ++i) {

        t = color & mask ? CYCLES_800_T1H : CYCLES_800_T0H;

        while ((c = ESP.getCycleCount()) - start < CYCLES_800); // wait for bit start
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pin_mask);        // then set LED pin to HIGH

        start = c;
        
        while (ESP.getCycleCount() - start < t);         // wait for HIGH duration
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pin_mask); // then set LED pin to LOW

        mask >>= 1;

    }

    os_intr_unlock();

    _mcp->digitalWrite(_MCP23017_LED_LOCK_PIN, LOW); // close the transistor lock
    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pin_mask); // light off the onboard LED

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