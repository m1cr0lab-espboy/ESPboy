/**
 * ----------------------------------------------------------------------------
 * @file   ESPboy.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  ESPboy primary controller
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
#include "assets.h"

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

/**
 * @brief The main class of the library providing a driver to control the ESPboy handheld.
 * 
 * @details This driver controls the display, the status of the various push buttons
 *          and the NeoPixel LED.
 */
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
        void _showESPboyLogo(char const * const title = nullptr, uint16 const color = 0xffff);
        void _updateFPS();

        void _fadeInOut(uint16_t const wait_ms = 0);
        void _fade();

    public:

        /**
         * @brief MCP4725 DAC controller.
         */
        Adafruit_MCP4725 dac;

        /**
         * @brief MCP23017 I/O expander controller.
         */
        Adafruit_MCP23X17 mcp;

        /**
         * @brief Display controller.
         */
        LGFX tft;

        /**
         * @brief Push button controller.
         */
        Button button;

        /**
         * @brief NeoPixel LED controller.
         */
        NeoPixel pixel;

        /**
         * @brief Initializes the ESPboy driver.
         * 
         * @param title Application name to display on startup screen.
         * @param color Title display color in 16-bit format (RGB565).
         */
        void begin(__FlashStringHelper const * const title, uint16 const color = 0xffff);
        
        /**
         * @brief Initializes the ESPboy driver.
         * 
         * @param title Application name to display on startup screen.
         * @param color Title display color in 16-bit format (RGB565).
         */
        void begin(char const * const title = nullptr, uint16 const color = 0xffff);
        
        /**
         * @brief Initializes the ESPboy driver by displaying a custom monochromatic logo at startup.
         * 
         * @param logo_width  Logo width.
         * @param logo_height Logo height.
         * @param logo_bitmap An array of 8-bit integers defining the logo bitmap.
         * @param logo_color  Logo display color in 16-bit format (RGB565).
         * @param wait_ms     Time length in milliseconds during which the logo must remain displayed.
         */
        void begin(
            uint8_t  const logo_width,
            uint8_t  const logo_height,
            uint8_t  const * const logo_bitmap,
            uint16_t const logo_color = 0xffff,
            uint16_t const wait_ms = 1000
        );
        
        /**
         * @brief Initializes the ESPboy driver by displaying a custom 16-bit color logo at startup.
         * 
         * @param logo_width    Logo width.
         * @param logo_height   Logo height.
         * @param logo_colormap An array of 16-bit integers defining the logo colormap in RGB565 space.
         * @param wait_ms       Time length in milliseconds during which the logo must remain displayed.
         */
        void begin(
            uint8_t  const logo_width,
            uint8_t  const logo_height,
            uint16_t const * const logo_colormap,
            uint16_t const wait_ms = 1000
        );

        /**
         * @brief Updates the ESPboy controller state.
         * 
         * @details Handles button reading and the NeoPixel LED state.
         */
        void update();

        /**
         * @brief Bulk read all push button states.
         * 
         * @return Current pin states of MCP23017 Port A as a uint8_t.
         * 
         * @details If the returned value is 0, it means that no button has been touched.
         */
        uint8_t buttons() const;

        /**
         * @brief Bulk read all push button states.
         * 
         * @return Current pin states of MCP23017 Port A as a uint8_t.
         * 
         * @details If the returned value is 0, it means that no button has been touched.
         * 
         * @note To please Roman 😉
         */
        uint8_t getKeys();

        /**
         * @brief Display frequency.
         * 
         * @return The current number of frames per second.
         */
        uint32_t fps() const;

        /**
         * @brief Current status of the screen brightness dimmer.
         * 
         * @return true if the screen brightness dimmer is active,
         *         false otherwise.
         */
        bool fading() const;

        /**
         * @brief Turns on the screen gradually.
         */
        void fadeIn();

        /**
         * @brief Turns off the screen gradually.
         */
        void fadeOut();

        /**
         * @brief Sets the brightness level of the screen.
         * 
         * @param brightness Screen brightness ranging from 0 (turned off) to 4095 (fully enlightened).
         */
        void dim(uint16_t const brightness);

};

/**
 * @brief An object created to provide full control of the ESPboy handheld.
 */
extern ESPboy espboy;

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