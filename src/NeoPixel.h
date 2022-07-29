/**
 * ----------------------------------------------------------------------------
 * @file   NeoPixel.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  NeoPixel controller
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "Color.h"

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
        uint32_t _fx_color;
        uint32_t _fx_start_ms;
        uint16_t _fx_duration_ms;
        uint16_t _fx_period_ms;
        uint16_t _fx_offset;
        uint8_t  _fx_count;
        bool     _fx_looping;
        bool     _flashing;

        uint32_t _rgb2grb(uint32_t const rgb) const;

        void _flash();
        void _breathe();
        void _rainbow();

        uint8_t _sine(uint8_t const i) const;

        // color must be in GRB888 format => 0x00GGRRBB
        void IRAM_ATTR _show(uint32_t const color) const;

    public:

        /**
         * @brief Initializes the NeoPixel LED.
         * 
         * @param mcp Reference to the MCP23017 controller owned by the espboy instance.
         */
        void begin(Adafruit_MCP23X17 &mcp);

        /**
         * @brief Updates the NeoPixel LED status.
         */
        void update();

        /**
         * @brief Sets the brightness of the NeoPixel LED.
         * 
         * @param b Brightness ranging from 0 to 255.
         */
        void setBrightness(uint8_t const b);

        /**
         * @brief Simply turn off the NeoPixel LED.
         */
        void clear() const;

        /**
         * @brief Turns off the NeoPixel LED and cancels all current lighting effects.
         */
        void reset();

        /**
         * @brief Applies a RGB888 color to the NeoPixel LED.
         * 
         * @param color RGB888 color to apply (0 to turn off the LED).
         */
        void show(uint32_t const color) const;
        
        /**
         * @brief Makes the NeoPixel LED blink.
         * 
         * @param color RGB888 color of each light flash.
         * @param duration_ms Duration in milliseconds of each light flash.
         * @param count Number of light flashes (1 by default, 0 to hold the effect indefinitely).
         * @param period_ms Length of time in milliseconds elapsed between each flash start.
         */
        void flash(uint32_t const color, uint16_t const duration_ms, uint8_t const count = 1, uint16_t const period_ms = 0);
        
        /**
         * @brief Dims the NeoPixel LED with sine wave cycles.
         * 
         * @param color RGB888 color of the light effect.
         * @param period_ms Sine wave period in milliseconds.
         * @param count Number of cycles (1 by default, 0 to hold the effect indefinitely).
         */
        void breathe(uint32_t const color, uint16_t const period_ms = 1000, uint8_t const count = 1);
        
        /**
         * @brief Applies a rainbow color cycle to the NeoPixel LED.
         * 
         * @param period_ms Period in milliseconds of a full color cycle.
         * @param count Number of cycles (1 by default, 0 to hold the effect indefinitely).
         */
        void rainbow(uint16_t const period_ms = 1000, uint8_t const count = 1);

};

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