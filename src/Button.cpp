/**
 * ----------------------------------------------------------------------------
 * @file   Button.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  ESPboy button controller
 * ----------------------------------------------------------------------------
 */

#include "Button.h"

void Button::read(uint8_t const input) {

    for (uint8_t i = 0; i < 8; ++i) {

        uint8_t const reading = input & (1 << i);

        /**
         * @note Software debouncing algorithm (Kennet A. Kuhn)
         * @see  https://www.kennethkuhn.com/electronics/debounce.c
         */
             if (!reading) { if (_integrator[i]) _integrator[i]--; }
        else if (_integrator[i] < _DEBOUNCING_THRESHOLD) _integrator[i]++;
        uint8_t const output = !_integrator[i] ? 0 : (_integrator[i] == _DEBOUNCING_THRESHOLD ? 1 : 0);

        switch (_state[i]) {

            case _State::FREE:
                if (output) _state[i] = _State::PRESSED;
                break;
            
            case _State::PRESSED:
                if (output) {
                    _state[i] = _State::HELD;
                    _held_start_ms[i] = millis();
                } else _state[i] = _State::RELEASED;
                break;
            
            case _State::HELD:
                if (!output) _state[i] = _State::RELEASED;
                break;
            
            case _State::RELEASED:
                _state[i] = _State::FREE;
                break;
            
        }

    }

}

bool Button::pressed(uint8_t const button)  const { return _state[button & 0x7] == _State::PRESSED;  }
bool Button::released(uint8_t const button) const { return _state[button & 0x7] == _State::RELEASED; }

bool Button::held(uint8_t const button, uint32_t const delay_ms) const {

    return _state[button & 0x7] == _State::HELD && (
        delay_ms
        ? millis() - _held_start_ms[button & 0x7] >= delay_ms
        : true
    );

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