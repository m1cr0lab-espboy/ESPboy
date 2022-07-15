/*
 * ----------------------------------------------------------------------------
 * ESPboy handheld buttons
 * ----------------------------------------------------------------------------
 * Copyright (c) 2021 Stéphane Calderoni (https://github.com/m1cr0lab)
 * ----------------------------------------------------------------------------
 * How to deal with buttons
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

LGFX_Sprite fb(&espboy.tft);

uint16_t pressed  = 0;
uint16_t held     = 0;
uint16_t released = 0;

void clear() { pressed = held = released = 0; }

void setup() {

    espboy.begin(false);
    fb.createSprite(29, 31);

    espboy.tft.setTextColor(TFT_LIGHTGRAY);
    espboy.tft.drawCenterString("Press ACT", 64, 16);

    espboy.tft.setTextColor(TFT_DARKGRAY);
    espboy.tft.drawString("Pressed",  22, 40);
    espboy.tft.drawString("Held",     22, 52);
    espboy.tft.drawString("Released", 22, 64);

    espboy.tft.setTextColor(TFT_LIGHTGRAY);
    espboy.tft.drawCenterString("Reset by holding", 64, 88);
    espboy.tft.drawCenterString("ESC down for 1 sec", 64, 100);

}

void loop() {

    espboy.update();
    
    fb.fillSprite(0);

    // The predefined buttons are as follows:
    // 
    //   Button::TOP_LEFT
    //   Button::TOP_RIGHT
    //   Button::LEFT
    //   Button::RIGHT
    //   Button::UP
    //   Button::DOWN
    //   Button::ACT
    //   Button::ESC
    // 
    // To find out if any of these buttons have been pressed, you can also use:
    // 
    //   if (espboy.buttons()) { ... }

    if (espboy.button.pressed(Button::ACT))    pressed++;
    if (espboy.button.held(Button::ACT))       held++;
    if (espboy.button.released(Button::ACT)) { released++; held = 0; }

    if (espboy.button.held(Button::ESC, 1000)) clear();

    fb.drawNumber(pressed,  0, 0);
    fb.drawNumber(held,     0, 12);
    fb.drawNumber(released, 0, 24);

    fb.pushSprite(82, 40);

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