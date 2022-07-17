/*
 * ----------------------------------------------------------------------------
 * ESPboy Library
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * ESPboy Logo
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <Arduino.h>

static uint8_t constexpr ESPBOY_LOGO_WIDTH  PROGMEM = 21;
static uint8_t constexpr ESPBOY_LOGO_HEIGHT PROGMEM = 36;

static uint8_t const constexpr ESPBOY_LOGO[] PROGMEM = {

    0x70, 0x00, 0x70,
    0xf8, 0x00, 0xf8,
    0xf8, 0xf8, 0xf8,
    0xb9, 0xfc, 0xb8,
    0x71, 0xfc, 0x70,
    0x01, 0x7c, 0x00,
    0x01, 0x7c, 0x00,
    0x71, 0x9c, 0x70,
    0xf8, 0xf8, 0xf8,
    0xf8, 0x00, 0xf8,
    0xb8, 0x00, 0xb8,
    0x70, 0x70, 0x70,
    0x00, 0xf8, 0x00,
    0x00, 0xf8, 0x00,
    0x70, 0xb8, 0x70,
    0xf8, 0x70, 0xf8,
    0xf8, 0x00, 0xf8,
    0xb8, 0x00, 0xb8,
    0x70, 0x70, 0x70,
    0x00, 0xf8, 0x00,
    0x00, 0xf8, 0x00,
    0x00, 0xb8, 0x00,
    0x00, 0x70, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x0e, 0x03, 0x80,
    0x1f, 0x07, 0xc0,
    0x1f, 0x07, 0xc0,
    0x17, 0x05, 0xc0,
    0x0e, 0x03, 0x80,
    0x00, 0x00, 0x00,
    0x38, 0x00, 0xe0,
    0x7c, 0x01, 0xf0,
    0x7c, 0x01, 0xf0,
    0x5c, 0x01, 0x70,
    0x38, 0x00, 0xe0

};

static uint8_t constexpr M1CR0LAB_WIDTH  PROGMEM = 42;
static uint8_t constexpr M1CR0LAB_HEIGHT PROGMEM = 5;

static uint8_t const constexpr M1CR0LAB[] PROGMEM = {

    0x5b, 0x39, 0xe7, 0xa0, 0xf3, 0x80,
    0xa9, 0x42, 0x28, 0xa1, 0x14, 0x80,
    0xa9, 0x42, 0x2a, 0xa1, 0x17, 0x80,
    0xa9, 0x43, 0xc8, 0xa1, 0xf4, 0x40,
    0xa9, 0x3a, 0x27, 0x3d, 0x17, 0x80

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