/**
 * ----------------------------------------------------------------------------
 * @file   Color.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Toolbox for simplified color handling
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>

class Color {

    private:

        static void _hsv2rgb(uint16_t hue, uint8_t const sat, uint8_t const val, uint8_t *red, uint8_t *green, uint8_t *blue);

    public:

        /**
         * @brief Returns a packed 32-bit RGB888 color from its red, green and blue components.
         * 
         * @param red Red level ranging from 0 to 255.
         * @param green Green level ranging from 0 to 255.
         * @param blue Blue level ranging from 0 to 255.
         * 
         * @return A 32-bit integer RGB888 color code.
         */
        static uint32_t rgb(uint8_t const red, uint8_t const green, uint8_t const blue);

        /**
         * @brief Returns a packed 16-bit RGB565 color from its red, green and blue components.
         * 
         * @param red Red level ranging from 0 to 255.
         * @param green Green level ranging from 0 to 255.
         * @param blue Blue level ranging from 0 to 255.
         * 
         * @return A 16-bit integer RGB565 color code.
         */
        static uint16_t rgb565(uint8_t const red, uint8_t const green, uint8_t const blue);

        /**
         * @brief Returns a packed 32-bit RGB888 color from its hue, saturation and brightness components.
         * 
         * @param hue Hue ranging from 0 to 359.
         * @param sat Saturation ranging from 0 to 255.
         * @param val Brightness ranging from 0 to 255.
         * 
         * @return A 32-bit integer RGB888 color code.
         */
        static uint32_t hsv2rgb(uint16_t hue, uint8_t const sat = 0xff, uint8_t const val = 0xff);

        /**
         * @brief Returns a packed 16-bit RGB565 color from its hue, saturation and brightness components.
         * 
         * @param hue Hue ranging from 0 to 359.
         * @param sat Saturation ranging from 0 to 255.
         * @param val Brightness ranging from 0 to 255.
         * 
         * @return A 16-bit integer RGB565 color code.
         */
        static uint16_t hsv2rgb565(uint16_t hue, uint8_t const sat = 0xff, uint8_t const val = 0xff);

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