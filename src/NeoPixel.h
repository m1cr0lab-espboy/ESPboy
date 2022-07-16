/*
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

/**
 * @brief This class provides a driver to control
 *        the NeoPixel LED of the EPboy handheld.
 */
class NeoPixel {

    private:

        static uint8_t const constexpr _FAST_SINE[] PROGMEM = {

              0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   6,   6,   8,   9,
             10,  11,  12,  14,  15,  17,  18,  20,  22,  23,  25,  27,  29,  31,  33,  35,
             38,  40,  42,  45,  47,  49,  52,  54,  57,  60,  62,  65,  68,  71,  73,  76,
             79,  82,  85,  88,  91,  94,  97, 100, 103, 106, 109, 113, 116, 119, 122, 125

        };

        static uint8_t constexpr _LED_PIN               = D4;
        static uint8_t constexpr _MCP23017_LED_LOCK_PIN = 9;

        static uint8_t constexpr CPU_FREQ       = F_CPU / 80000000L;
        static uint8_t constexpr CYCLES_800_T0H = CPU_FREQ * 32;
        static uint8_t constexpr CYCLES_800_T1H = CPU_FREQ * 64;
        static uint8_t constexpr CYCLES_800     = CPU_FREQ * 100;

        enum class _FX : uint8_t {

            NONE,
            FLASH,
            BREATHE,
            RAINBOW

        };

        Adafruit_MCP23X17 *_mcp;

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

        uint8_t _sine(uint8_t const i) const;

    public:

        /**
         * @brief Initializes the NeoPixel LED.
         * 
         * @param mcp The reference to the MCP23017 controller owned by the espboy instance.
         */
        void begin(Adafruit_MCP23X17 &mcp);

        /**
         * @brief Updates the NeoPixel LED status.
         */
        void update();

        /**
         * @brief Sets the brightness of the NeoPixel LED.
         * 
         * @param b An integer ranging from 0 to 255.
         */
        void setBrightness(uint8_t const b);

        /**
         * @brief Returns a RGB888 color from its red, green and blue components.
         * 
         * @param red The intensity of the red component, ranging from 0 to 255.
         * @param green The intensity of the green component, ranging from 0 to 255.
         * @param blue The intensity of the blue component, ranging from 0 to 255.
         * 
         * @return A 32-bit integer corresponding to the RGB888 color code.
         */
        uint32_t rgb(uint8_t const red, uint8_t const green, uint8_t const blue) const;

        /**
         * @brief Returns a RGB888 color from its hue, saturation and value components.
         * 
         * @param hue The color hue, ranging from 0 to 359.
         * @param sat The color saturation, ranging from 0 to 255.
         * @param val The color value, ranging from 0 to 255.
         * 
         * @return A 32-bit integer corresponding to the RGB888 color code.
         */
        uint32_t hsv(uint16_t hue, uint8_t const sat = 0xff, uint8_t const val = 0xff) const;

        /**
         * @brief Simply turn off the NeoPixel LED.
         */
        void clear() const;

        /**
         * @brief Turns off the NeoPixel LED and cancels all current lighting effects.
         */
        void reset();
        
        /**
         * @brief Makes the NeoPixel LED strobe.
         * 
         * @param color The RGB888 color of each light flash.
         * @param duration_ms The duration in milliseconds of each light flash.
         * @param count The number of light flashes.
         * @param period_ms The time in milliseconds elapsed between the start of each light flash.
         */
        void flash(uint32_t const color, uint16_t const duration_ms, uint8_t const count = 1, uint16_t const period_ms = 0);
        
        /**
         * @brief Makes the NeoPixel LED dim in a sine wave pattern.
         * 
         * @param color The RGB888 color of the light effect.
         * @param wait_ms The length of time each brightness level is maintained.
         * @param count The number of sine waves.
         */
        void breathe(uint32_t const color, uint16_t const wait_ms = 5, uint8_t const count = 1);
        
        /**
         * @brief Applies a rainbow-like color pattern to the NeoPixel LED.
         * 
         * @param wait_ms The length of time each color hue is maintained.
         * @param count The number of periodic patterns.
         */
        void rainbow(uint32_t const wait_ms = 5, uint8_t const count = 1);

        /**
         * @brief Applies a color to the NeoPixel LED.
         * 
         * @param color The RGB888 color to apply (0 to turn off the LED).
         */
        void IRAM_ATTR show(uint32_t const color) const;

};

/*
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