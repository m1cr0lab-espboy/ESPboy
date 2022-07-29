/**
 * ----------------------------------------------------------------------------
 * @file   4-neopixel.ino
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  The different ways to handle the NeoPixel RGB LED.
 * 
 * @note   To set the LED brightness:
 * 
 *           espboy.pixel.setBrightness(level)
 * 
 *             where level is ranging from 0 to 255 (default: 0x20 = 32).
 * 
 *         To turn the LED on:
 * 
 *           espboy.pixel.show(color)
 * 
 *             where color is a packed 32-bit integer defining a RGB888 color.
 * 
 *         To turn the LED off:
 * 
 *           The following statements are equivalent:
 * 
 *             espboy.pixel.clear()
 *             espboy.pixel.show(O)
 * 
 *         --------
 *         Light FX
 *         --------
 * 
 *         To make the LED blink:
 * 
 *           espboy.pixel.flash(color, duration, count, period)
 * 
 *             where:
 *               - color:    a packed 32-bit integer defining a RGB888 color.
 *               - duration: the duration in milliseconds of each light flash.
 *               - count:    the number of light flashes (1 is set by default,
 *                           but you can set 0 if you want the LED blink
 *                           indefinitely).
 *               - period:   the length of time in milliseconds elapsed between
 *                           each flash start (0 by default).
 * 
 *         To dim the LED in sine wave cycles:
 * 
 *           espboy.pixel.breathe(color, period, count)
 * 
 *             where:
 *               - color:  a packed 32-bit integer defining a RGB888 color.
 *               - period: the sine wave period in milliseconds (1000 ms by default).
 *               - count:  the number of cycles (1 is set by default,
 *                         but you can set 0 if you want the LED brightness
 *                         to vary indefinitely).
 * 
 *         To apply a beautiful rainbow color cycle to the NeoPixel LED:
 * 
 *           espboy.pixel.rainbow(period, count)
 * 
 *             where:
 *               - period: the period in milliseconds of a full color cycle
 *                         from hue 0 to 359 (1000 ms by default).
 *               - count:  the number of cycles (1 is set by default,
 *                         but you can set 0 if you want the effect to last
 *                         indefinitely).
 * 
 *         To cancel any light effect (flash, breathe, or rainbow) and turn the LED off:
 * 
 *           espboy.pixel.reset();
 * 
 * @details Note that for color settings you can use the Color toolbox:
 * 
 *            Color::rgb(red, green, blue)
 * 
 *              where:
 *                - red:   the red level   (ranging from 0 to 255).
 *                - green: the green level (ranging from 0 to 255).
 *                - blue:  the blue level  (ranging from 0 to 255).
 * 
 *            Color::hsv2rgb(hue, saturation, brightness)
 * 
 *              where:
 *                - hue:   the color hue        (ranging from 0 to 359).
 *                - green: the color saturation (ranging from 0 to 255, 0xff by default).
 *                - blue:  the color brightness (ranging from 0 to 255, 0xff by default).
 * 
 *            Both methods return a packed 32-bit integer RGB888 color code.
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

uint8_t brightness;

void setup() {

    espboy.begin();

    espboy.tft.drawCenterString(F("NeoPixel Effects"), TFT_WIDTH >> 1, 18);

    uint8_t y;
    espboy.tft.setTextColor(TFT_LIGHTGRAY);
    espboy.tft.drawString(F("LEFT  Slow Flashing"), 8, y  = 42);
    espboy.tft.drawString(F("RIGHT Fast Flashing"), 8, y += 12);
    espboy.tft.drawString(F("UP    Breathe"),       8, y += 12);
    espboy.tft.drawString(F("DOWN  Rainbow"),       8, y += 12);
    espboy.tft.drawString(F("ACT   + Brightness"),  8, y += 12);
    espboy.tft.drawString(F("ESC   - Brightness"),  8, y += 12);

    espboy.pixel.setBrightness(brightness = 0x40);

}

void loop() {

    espboy.update();

    if (espboy.buttons()) {

        static uint16_t hue = 0;

        hue = (hue + 20 * random(1, 10)) % 360;

        uint32_t color  = Color::hsv2rgb(hue);
        uint16_t period = 1 << random(8, 12);

             if (espboy.button.pressed(Button::LEFT))  espboy.pixel.flash(color, 100, 0, 1000);
        else if (espboy.button.pressed(Button::RIGHT)) espboy.pixel.flash(color,  50, 0,  250);
        else if (espboy.button.pressed(Button::UP))    espboy.pixel.breathe(color, period, 0);
        else if (espboy.button.pressed(Button::DOWN))  espboy.pixel.rainbow(period, 0);
        else {

            static uint32_t last = millis();
            uint32_t const now   = millis();

            if (now - last > 5) {

                     if (espboy.button.held(Button::ACT) && (brightness < 0xff)) espboy.pixel.setBrightness(brightness++);
                else if (espboy.button.held(Button::ESC) && (brightness > 0x00)) espboy.pixel.setBrightness(brightness--);

                last = now;

            }

        }

    }

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