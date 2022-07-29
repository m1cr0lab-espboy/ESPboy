/**
 * ----------------------------------------------------------------------------
 * @file   Color.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Toolbox for simplified color handling
 * ----------------------------------------------------------------------------
 */

#include "Color.h"

void Color::_hsv2rgb(uint16_t hue, uint8_t const sat, uint8_t const val, uint8_t *red, uint8_t *green, uint8_t *blue) {

    if (!sat) { *red = *green = *blue = val; return; }

    hue = (hue << 5) / 45; // converts [0, 359] to [0, 255]

    uint8_t const sextant   = hue / 43;
    uint8_t const remainder = (hue - (sextant * 43)) * 6;

    uint8_t const p = (val * ~sat) >> 8;
    uint8_t const q = (val * ~(sat *  remainder) >> 8) >> 8;
    uint8_t const t = (val * ~(sat * ~remainder) >> 8) >> 8;

    switch (sextant) {

        case 0:  *red = val; *green =   t; *blue =   p; break;
        case 1:  *red =   q; *green = val; *blue =   p; break;
        case 2:  *red =   p; *green = val; *blue =   t; break;
        case 3:  *red =   p; *green =   q; *blue = val; break;
        case 4:  *red =   t; *green =   p; *blue = val; break;
        default: *red = val; *green =   p; *blue =   q;

    }

}

uint32_t Color::rgb(uint8_t const red, uint8_t const green, uint8_t const blue) {

    return (red << 16) | (green << 8) | blue;

}

uint16_t Color::rgb565(uint8_t const red, uint8_t const green, uint8_t const blue) {

    return (red >> 3) << 11 | (green >> 2) << 5 | blue >> 3;

}

uint32_t Color::hsv2rgb(uint16_t hue, uint8_t const sat, uint8_t const val) {

    uint8_t red, green, blue;

    _hsv2rgb(hue, sat, val, &red, &green, &blue);

    return rgb(red, green, blue);

}

uint16_t Color::hsv2rgb565(uint16_t hue, uint8_t const sat, uint8_t const val) {

    uint8_t red, green, blue;

    _hsv2rgb(hue, sat, val, &red, &green, &blue);

    return rgb565(red, green, blue);

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