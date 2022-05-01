/**
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * ESPboy Button Controller
 * ----------------------------------------------------------------------------
 */
#include "Button.h"

void Button::read(const uint8_t input) {

    for (uint8_t i = 0; i < 8; ++i) {

        const uint8_t reading = input & (1 << i);

        /**
         * Software debouncing algorithm (Kennet A. Kuhn)
         * @see https://www.kennethkuhn.com/electronics/debounce.c
         */
             if (!reading) { if (_integrator[i]) _integrator[i]--; }
        else if (_integrator[i] < _DEBOUNCING_THRESHOLD) _integrator[i]++;
        const uint8_t output = !_integrator[i] ? 0 : (_integrator[i] == _DEBOUNCING_THRESHOLD ? 1 : 0);

        switch (_state[i])
        {
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

bool Button::pressed(const uint8_t button)  const { return _state[button & 0x7] == _State::PRESSED;  }
bool Button::released(const uint8_t button) const { return _state[button & 0x7] == _State::RELEASED; }

bool Button::held(const uint8_t button, const uint32_t delay_ms) const {

    return _state[button & 0x7] == _State::HELD && (
        delay_ms
        ? millis() - _held_start_ms[button & 0x7] >= delay_ms
        : true
    );

}

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