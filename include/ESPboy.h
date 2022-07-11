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

// To please Roman
constexpr uint8_t PAD_LEFT  = 0x01;
constexpr uint8_t PAD_UP    = 0x02;
constexpr uint8_t PAD_DOWN  = 0x04;
constexpr uint8_t PAD_RIGHT = 0x08;
constexpr uint8_t PAD_ACT   = 0x10;
constexpr uint8_t PAD_ESC   = 0x20;
constexpr uint8_t PAD_LFT   = 0x40;
constexpr uint8_t PAD_RGT   = 0x80;
constexpr uint8_t PAD_ANY   = 0xff;

constexpr uint8_t TFT_WIDTH  = 128;
constexpr uint8_t TFT_HEIGHT = 128;

class ESPboy {

    private:

        static constexpr uint8_t _MCP23017_TFT_CS_PIN = 8;

        static constexpr uint16_t _DAC_MIN = 650;
        static constexpr uint16_t _DAC_MAX = 1000;

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
        void _initMCP4725();
        void _initMCP23017();
        void _initTFT();
        void _updateFPS();

        void _fadeInOut(const uint16_t wait_ms = 0);
        void _fade();

    public:

        Adafruit_MCP4725  dac;
        Adafruit_MCP23X17 mcp;

        LGFX     tft;
        Button   button;
        NeoPixel pixel;

        void begin(const bool show_espboy_logo = true, const uint16_t wait_ms = 1000);
        void begin(const uint8_t logo_width, const uint8_t logo_height, const uint16_t *logo_data, const uint16_t wait_ms = 1000);
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