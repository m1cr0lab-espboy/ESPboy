/**
 * ----------------------------------------------------------------------------
 * @file   3-buttons.ino
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  The different ways to handle push buttons.
 * 
 * @note   Each button is identified by a unique reference constant:
 * 
 *           Button::LEFT
 *           Button::UP
 *           Button::DOWN
 *           Button::RIGHT
 *           Button::ACT
 *           Button::ESC
 *           Button::TOP_LEFT
 *           Button::TOP_RIGHT
 * 
 *         To find out if any button has been pressed:
 * 
 *           if (espboy.buttons()) { ... }
 * 
 *         To find out if a button has just been pressed:
 * 
 *           if (espboy.button.pressed(Button::ACT)) { ... }
 * 
 *         To find out if a button is held down (possibly for a given time).
 * 
 *           if (espboy.button.held(Button::ACT), delay) { ... }
 * 
 *             where delay is the length of time in milliseconds that the
 *             button must be held down, or 0 (default) if you just want to
 *             know if the button is still pressed.
 * 
 *         To find out if a button has just been released:
 * 
 *           if (espboy.button.released(Button::ACT)) { ... }
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

LGFX_Sprite fb(&espboy.tft);

uint16_t pressed  = 0;
uint16_t held     = 0;
uint16_t released = 0;

void clear() { pressed = held = released = 0; }

void setup() {

    espboy.begin();
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