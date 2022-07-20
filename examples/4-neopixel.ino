/*
 * ----------------------------------------------------------------------------
 * ESPboy handheld NeoPixel LED
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * How to use the NeoPixel RGB LED
 * ----------------------------------------------------------------------------
 * To set the brightness of the LED:
 *   espboy.pixel.setBrightness(value)
 *     where value is ranging from 0 to 255 (default: 0x20 = 32)
 * ----------------------------------------------------------------------------
 * To light on the LED:
 *   espboy.pixel.show(rgb888_color)
 * ----------------------------------------------------------------------------
 * To light off the LED:
 *   espboy.pixel.clear(), which is equivalent to:
 *   espboy.pixel.show(O)
 * ----------------------------------------------------------------------------
 * To make the LED blink:
 *   espboy.pixel.flash(rgb888_color, duration_ms, count, period_ms)
 *   - rgb888_color: 0xff00ff for example
 *   - duration_ms:  the duration in milliseconds of each light flash
 *   - count:        the number of light flashes
 *   - period_ms:    the time in milliseconds between each flash start
 * ----------------------------------------------------------------------------
 * To make the LED dim in a sine wave pattern:
 *   espboy.pixel.flash(rgb888_color, wait_ms, count)
 *   - rgb888_color: 0xff00ff for example
 *   - wait_ms:      the length of time each brightness level is maintained
 *   - count:        the number of light flashes
 * ----------------------------------------------------------------------------
 * To apply a rainbow-like color pattern to the LED:
 *   espboy.pixel.rainbow(wait_ms, count)
 *   - wait_ms:      the length of time each color hue is maintained
 *   - count:        the number of periodic patterns
 * ----------------------------------------------------------------------------
 * A color in RGB888 format can be obtained by one of these methods:
 *   - in packed 32-bit format: 0x[00]rrggbb
 *   - Color::rgb(red, green, blue)
 *       where red, green and blue are ranging from 0 to 255 (0xff)
 *   - Color::hsv2rgb(hue, saturation, value)
 *       where:
 *         - hue is ranging from 0 to 359
 *         - saturation is ranging from 0 to 255 (default: 0xff = 255)
 *         - value (brightness) is ranging from 0 to 255 (default: 0xff = 255)
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
        
        uint32_t color = Color::hsv2rgb(hue);
        uint8_t  wait  = random(1, 6) << 1;

             if (espboy.button.pressed(Button::LEFT))  espboy.pixel.flash(color, 100, 0, 1000);
        else if (espboy.button.pressed(Button::RIGHT)) espboy.pixel.flash(color,  50, 0,  250);
        else if (espboy.button.pressed(Button::UP))    espboy.pixel.breathe(color, wait, 0);
        else if (espboy.button.pressed(Button::DOWN))  espboy.pixel.rainbow(wait, 0);
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