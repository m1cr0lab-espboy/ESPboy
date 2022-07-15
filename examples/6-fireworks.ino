/**
 * ----------------------------------------------------------------------------
 * Fireworks: a nice example with particle projection
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

uint8_t constexpr MAX_FIREWORKS = 5;
uint8_t constexpr MAX_SPARKLES  = 40;
float_t constexpr GRAVITY       = .25f;

LGFX_Sprite fb(&espboy.tft);

uint16_t rgb565(uint8_t const red, uint8_t const green, uint8_t const blue) {

    return ((red   & 0xf8) << 8) |
           ((green & 0xfc) << 3) |
            (blue >> 3);

}

uint16_t hsv(uint16_t hue, uint8_t const sat = 0xff, uint8_t const val = 0xff) {

    if (!sat) return rgb565(val, val, val);

    hue = (hue << 5) / 45;

    uint8_t r, g, b;

    uint8_t const sextant   = hue / 43;
    uint8_t const remainder = (hue - (sextant * 43)) * 6;

    uint8_t const p = (val * ~sat) >> 8;
    uint8_t const q = (val * ~(sat *  remainder) >> 8) >> 8;
    uint8_t const t = (val * ~(sat * ~remainder) >> 8) >> 8;

    switch (sextant) {

        case 0:  r = val; g =   t; b =   p; break;
        case 1:  r =   q; g = val; b =   p; break;
        case 2:  r =   p; g = val; b =   t; break;
        case 3:  r =   p; g =   q; b = val; break;
        case 4:  r =   t; g =   p; b = val; break;
        default: r = val; g =   p; b =   q;

    }

    return rgb565(r, g, b);

}

struct Sparkle {

    bool     fired;
    bool     first;
    float_t  x;
    float_t  y;
    float_t  vx;
    float_t  vy;
    uint16_t hue;
    uint8_t  lifespan;

    void fire(uint8_t const x, uint8_t const y, uint16_t const hue, bool const first = false) {

        fired       = true;
        this->first = first;
        this->x     = x;
        this->y     = y;
        vx          = (random(2) ? -1 : 1) * (random(first ? 20 : 30) / 10.f);
        vy          = first ? -4 - (random(30) / 10.f) : -1 - (random(40) / 10.f);
        this->hue   = hue;
        lifespan    = 0xff;

    }

    void update() {

        x += vx;
        y += vy;

        if (x < 0 || x + 1 > TFT_WIDTH || y + 1 > TFT_HEIGHT) {
            fired = false;
            return;
        }

        vy += GRAVITY;

        lifespan > 3
            ? lifespan -= 3
            : fired = false;

    }

    void draw() {

        first
            ? fb.fillRect(x - 1, y - 1, 3, 3, hsv(hue))
            : fb.fillRect(x, y, 2, 2, hsv(hue, 0xff, lifespan));

    }

};

struct Firework {

    uint16_t hue;
    Sparkle  sparkles[MAX_SPARKLES];

    void fire() { sparkles[0].fire(TFT_WIDTH >> 1, TFT_HEIGHT - 1, (hue = random(360)), true); }

    void update() {

        for (uint8_t i = 0; i < MAX_SPARKLES; ++i) {

            Sparkle * const s = &sparkles[i];

            if (s->fired) s->update();
            if (s->first && s->vy > 0) {
                espboy.pixel.flash(espboy.pixel.hsv(hue), 50);
                for (uint8_t j = 0; j < MAX_SPARKLES; ++j) {
                    sparkles[j].fire(s->x, s->y, s->hue);
                }
            }

        }

    }

    void draw() {

        for (uint8_t i = 0; i < MAX_SPARKLES; ++i) {
            Sparkle * const s = &sparkles[i];
            if (s->fired) s->draw();
        }

    }

};

Firework fireworks[MAX_FIREWORKS];
uint8_t firework_index = 0;

void setup() {

    espboy.begin();
    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);

}

void loop() {

    espboy.update();

    if (!random(30)) {

        fireworks[firework_index++].fire();
        if (firework_index == MAX_FIREWORKS) firework_index = 0;

    }
    
    fb.clear();

    for (uint8_t i = 0; i < MAX_FIREWORKS; ++i) {

        Firework * const f = &fireworks[i];
        f->update();
        f->draw();

    }

    fb.pushSprite(0, 0);

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