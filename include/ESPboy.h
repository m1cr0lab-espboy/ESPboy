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

constexpr uint8_t TFT_WIDTH  = 128;
constexpr uint8_t TFT_HEIGHT = 128;

class ESPboy {

    private:

        static constexpr uint16_t _DAC_MIN = 650;
        static constexpr uint16_t _DAC_MAX = 1000;

        struct Fading {

            uint32_t last_us;
            uint16_t level;
            bool     inc;
            bool     active;

        };
        
        Adafruit_MCP4725  _dac;
        Adafruit_MCP23X17 _mpx;

        uint8_t  _buttons;
        uint32_t _frame_count;
        uint32_t _fps;
        Fading   _fading;

        void _initTFT();
        void _initMCP4725();
        void _initMCP23017();
        void _updateFPS();

        void _drawLogo();
        void _drawLogo(const uint8_t width, const uint8_t height, const uint16_t *data);
        void _fade();

    public:

        LGFX     tft;
        Button   button;
        NeoPixel pixel;

        void begin(const bool show_espboy_logo = true, const uint16_t wait_ms = 0);
        void begin(const uint8_t logo_width, const uint8_t logo_height, const uint16_t *logo_data, const uint16_t wait_ms = 0);
        void update();

        uint8_t buttons() const;

        uint32_t fps() const;

        bool fading() const;
        void fadeIn();
        void fadeOut();

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