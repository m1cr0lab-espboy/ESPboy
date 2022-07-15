/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * ESPboy Button Controller
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <Arduino.h>

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

        void read(uint8_t const input);
        bool pressed(uint8_t const button) const;
        bool released(uint8_t const button) const;
        bool held(uint8_t const button, uint32_t const delay_ms = 0) const;
    
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