/**
 * ----------------------------------------------------------------------------
 * @file   Button.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  ESPboy button controller
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>

/**
 * @brief This class provides a controller to check the state
 *        of the various buttons of the ESPboy handheld.
 */
class Button {

    private:

        static uint8_t constexpr _DEBOUNCING_THRESHOLD = 3;

        enum class _State : uint8_t { FREE, PRESSED, HELD, RELEASED };

        _State   _state[8];
        uint32_t _held_start_ms[8];
        uint8_t  _integrator[8];

    public:

        static uint8_t constexpr LEFT      = 0;
        static uint8_t constexpr UP        = 1;
        static uint8_t constexpr DOWN      = 2;
        static uint8_t constexpr RIGHT     = 3;
        static uint8_t constexpr ACT       = 4;
        static uint8_t constexpr ESC       = 5;
        static uint8_t constexpr TOP_LEFT  = 6;
        static uint8_t constexpr TOP_RIGHT = 7;

        /**
         * @brief Bulk read all push button states.
         * 
         * @param input Current pin states of MCP23017 Port A as a uint8_t.
         * 
         * @details The state of each button is filtered by a software debouncing algorithm.
         */
        void read(uint8_t const input);

        /**
         * @brief Checks if a button has just been pressed.
         * 
         * @param button One of the predefined constants for each of the ESPboy buttons:
         *               Button::LEFT, Button::RIGHT, Button::UP, Button::DOWN,
         *               Button::ACT, Button::ESC, Button::TOP_LEFT or Button::TOP_RIGHT.
         * 
         * @return true if the button has just been pressed,
         *         false otherwise.
         */
        bool pressed(uint8_t const button) const;

        /**
         * @brief Checks if a button has just been released.
         * 
         * @param button One of the predefined constants for each of the ESPboy buttons:
         *               Button::LEFT, Button::RIGHT, Button::UP, Button::DOWN,
         *               Button::ACT, Button::ESC, Button::TOP_LEFT or Button::TOP_RIGHT.
         * 
         * @return true if the button has just been released,
         *         false otherwise.
         */
        bool released(uint8_t const button) const;

        /**
         * @brief Checks if a button is held down (possibly for a given time).
         * 
         * @param button One of the predefined constants for each of the ESPboy buttons:
         *               Button::LEFT, Button::RIGHT, Button::UP, Button::DOWN,
         *               Button::ACT, Button::ESC, Button::TOP_LEFT or Button::TOP_RIGHT.
         * 
         * @param delay_ms Length of time in milliseconds that the button must be held down,
         *                 or 0 if you just want to know if the button is still pressed.
         * 
         * @return true if the button is held down, taking into account a non-zero
         *         delay_ms if specified, false otherwise.
         */
        bool held(uint8_t const button, uint32_t const delay_ms = 0) const;
    
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