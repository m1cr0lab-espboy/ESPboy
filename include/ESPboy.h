/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * ESPboy Primary Controller
 * ----------------------------------------------------------------------------
 */
#pragma once

#define LGFX_ESPBOY
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP4725.h>
#include "Button.h"
#include "NeoPixel.h"
#include "espboy-logo.h"

// To please Roman 😉
uint8_t constexpr PAD_LEFT  = 0x01;
uint8_t constexpr PAD_UP    = 0x02;
uint8_t constexpr PAD_DOWN  = 0x04;
uint8_t constexpr PAD_RIGHT = 0x08;
uint8_t constexpr PAD_ACT   = 0x10;
uint8_t constexpr PAD_ESC   = 0x20;
uint8_t constexpr PAD_LFT   = 0x40;
uint8_t constexpr PAD_RGT   = 0x80;
uint8_t constexpr PAD_ANY   = 0xff;

uint8_t constexpr TFT_WIDTH  = 128;
uint8_t constexpr TFT_HEIGHT = 128;

class ESPboy {

    private:

        static uint8_t constexpr _MCP23017_TFT_CS_PIN = 8;

        static uint16_t constexpr _DAC_MIN = 650;
        static uint16_t constexpr _DAC_MAX = 1000;

        bool _initialized = false;

        struct Fading {

            uint32_t last_us;
            uint16_t level;
            bool     inc;
            bool     active;

        };

        uint8_t  _buttons;
        uint32_t _frame_count;
        uint32_t _fps;
        Fading   _fading;

        void _init();
        void _initMCP23017();
        void _updateFPS();

        void _fadeInOut(uint16_t const wait_ms = 0);
        void _fade();

    public:

        Adafruit_MCP4725  dac;
        Adafruit_MCP23X17 mcp;

        LGFX     tft;
        Button   button;
        NeoPixel pixel;

        void begin(bool const show_espboy_logo = true, uint16_t const wait_ms = 1000);
        void begin(uint8_t const logo_width, uint8_t const logo_height, uint16_t const * const logo_data, uint16_t const wait_ms = 1000);
        void update();

        uint8_t buttons() const;
        uint8_t getKeys() const;

        uint32_t fps() const;

        bool fading() const;
        void fadeIn();
        void fadeOut();
        void dim(uint16_t const level);

};

extern ESPboy espboy;

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