/**
 * ----------------------------------------------------------------------------
 * How to use the NeoPixel RGB LED
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

        // You can define an RGB or HSV color as follows:
        // 
        //   uint32_t color = espboy.pixel.rgb(uint8_t red, uint8_t green, uint8_t blue)
        //   uint32_t color = espboy.pixel.hsv(uint16_t hue, uint8_t sat = 0xff, uint8_t val = 0xff)
        // 
        // Then to set the color on the LED:
        // 
        //   espboy.pixel.show(color)

        hue = (hue + 20 * random(1, 10)) % 360;
        
        uint32_t color = espboy.pixel.hsv(hue);
        uint8_t  wait  = random(1, 6) << 1;

        // But you can also apply effects to the LED:

             if (espboy.button.pressed(Button::LEFT))  espboy.pixel.flash(color, 100, 0, 1000);
        else if (espboy.button.pressed(Button::RIGHT)) espboy.pixel.flash(color,  50, 0,  250);
        else if (espboy.button.pressed(Button::UP))    espboy.pixel.breathe(color, wait, 0);
        else if (espboy.button.pressed(Button::DOWN))  espboy.pixel.rainbow(wait, 0);
        else {

            static uint32_t last = millis();
            uint32_t const now   = millis();

            if (now - last > 5) {

                // Finally, you can adjust the brightness of the LED:

                     if (espboy.button.held(Button::ACT) && (brightness < 0xff)) espboy.pixel.setBrightness(brightness++);
                else if (espboy.button.held(Button::ESC) && (brightness > 0x00)) espboy.pixel.setBrightness(brightness--);

                last = now;

            }

        }

    }

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