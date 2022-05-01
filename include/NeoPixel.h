/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * NeoPixel Driver
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class NeoPixel {

    private:

        static constexpr uint8_t _FAST_SINE[] PROGMEM = {

              0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   6,   6,   8,   9,
             10,  11,  12,  14,  15,  17,  18,  20,  22,  23,  25,  27,  29,  31,  33,  35,
             38,  40,  42,  45,  47,  49,  52,  54,  57,  60,  62,  65,  68,  71,  73,  76,
             79,  82,  85,  88,  91,  94,  97, 100, 103, 106, 109, 113, 116, 119, 122, 125

        };

        static constexpr uint8_t _LED_PIN           = D4;
        static constexpr uint8_t _MCP23017_LOCK_PIN = 9;

        static constexpr uint8_t CPU_FREQ       = F_CPU / 80000000L;
        static constexpr uint8_t CYCLES_800_T0H = CPU_FREQ * 32;
        static constexpr uint8_t CYCLES_800_T1H = CPU_FREQ * 64;
        static constexpr uint8_t CYCLES_800     = CPU_FREQ * 100;

        enum class _FX : uint8_t {

            NONE,
            FLASH,
            BREATHE,
            RAINBOW

        };

        Adafruit_MCP23X17 *_mpx;

        uint8_t _brightness;

        _FX      _fx;
        uint16_t _fx_offset;
        uint16_t _fx_hue;
        uint32_t _fx_color;
        uint32_t _fx_start_ms;
        uint32_t _fx_duration_ms;
        uint32_t _fx_period_ms;
        uint8_t  _fx_count;
        bool     _fx_looping;
        bool     _flashing;

        void _flash();
        void _breathe();
        void _rainbow();

        uint8_t _sine(const uint8_t i) const;

    public:

        void begin(Adafruit_MCP23X17 &mpx);
        void update();

        void     setBrightness(const uint8_t b);
        uint32_t rgb(const uint8_t red, const uint8_t green, const uint8_t blue) const;
        uint32_t hsv(uint16_t hue, const uint8_t sat = 0xff, const uint8_t val = 0xff) const;

        
        void clear() const;
        void reset();
        
        void flash(const uint32_t color, const uint16_t duration_ms, const uint8_t count = 1, const uint16_t period_ms = 0);
        void breathe(const uint32_t color, const uint16_t wait_ms = 5, const uint8_t count = 1);
        void rainbow(const uint32_t wait_ms = 5, const uint8_t count = 1);

        void IRAM_ATTR show(const uint32_t color) const;

};

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