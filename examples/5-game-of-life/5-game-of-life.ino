/**
 * ----------------------------------------------------------------------------
 * @file   5-game-of-life.ino
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  A cellular automaton designed by John Horton Conway (1970).
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

LGFX_Sprite framebuffer(&espboy.tft);

uint8_t ng[TFT_WIDTH+2][TFT_HEIGHT+2];
uint8_t pg[TFT_WIDTH+2][TFT_HEIGHT+2];

void reset() {

    espboy.pixel.breathe(Color::hsv2rgb(120), 250, 2);

    for (uint8_t y = 1; y <= TFT_HEIGHT; ++y) {
        for (uint8_t x = 1; x <= TFT_WIDTH; ++x) {
            ng[x][y] = !random(5);
        }
    }

}

void update() {

    memcpy(pg, ng, sizeof(ng));

    for (uint8_t y = 1; y <= TFT_HEIGHT; ++y) {
        for (uint8_t x = 1; x <= TFT_WIDTH; ++x) {
            
            uint8_t const n = pg[x-1][y-1] + pg[x][y-1] + pg[x+1][y-1] +
                              pg[x-1][ y ]              + pg[x+1][ y ] +
                              pg[x-1][y+1] + pg[x][y+1] + pg[x+1][y+1];

            ng[x][y] = pg[x][y] ? n == 2 || n == 3 : n == 3;
        }
    }

}

void draw() {

    framebuffer.clear();

    for (uint8_t y = 1; y <= TFT_HEIGHT; ++y) {
        for (uint8_t x = 1; x <= TFT_WIDTH; ++x) {
            if (ng[x][y]) framebuffer.drawPixel(x-1, y-1, 0x1);
        }
    }

    framebuffer.pushSprite(0, 0);

}

void setup() {

    espboy.begin();

    framebuffer.setColorDepth(lgfx::palette_1bit);
    framebuffer.createSprite(TFT_WIDTH, TFT_HEIGHT);
    framebuffer.createPalette();
    framebuffer.setPaletteColor(1, 0x00, 0xff, 0x80);

    reset();
    draw();

}

void loop() {

    espboy.update();

    if (espboy.button.pressed(Button::ACT)) reset();

    update();
    draw();

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