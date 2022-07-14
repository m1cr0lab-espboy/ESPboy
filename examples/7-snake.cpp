/**
 * ----------------------------------------------------------------------------
 * The Snake game
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

static uint8_t constexpr SIZE = 5;
static uint8_t constexpr COLS = (TFT_WIDTH  - 4) / SIZE;
static uint8_t constexpr ROWS = (TFT_HEIGHT - 4) / SIZE;
static uint8_t constexpr OX   = (TFT_WIDTH  - COLS * SIZE) >> 1;
static uint8_t constexpr OY   = (TFT_HEIGHT - ROWS * SIZE) >> 1;

struct Point {

    uint8_t x, y;

    void draw(uint16_t const color) const {

        espboy.tft.fillRect(OX + x * SIZE, OY + y * SIZE, SIZE - 1, SIZE - 1, color);

    } 

};

struct Apple : Point {

    void draw() const { Point::draw(0xf80a); }

} apple;

struct Snake {

    static uint8_t  constexpr START_LENGTH = 3;
    static uint16_t constexpr MAX_LENGTH   = COLS * ROWS;
    static uint16_t constexpr COLOR        = 0x07ea;

    enum class Dir : uint8_t { UP, RIGHT, DOWN, LEFT };

    bool     is_dead;
    uint16_t head, length;
    Point    tail[MAX_LENGTH];
    Dir      dir, user_dir;

    uint16_t score() const { return length - START_LENGTH; }

    void reset() {

        is_dead = false;
        length  = START_LENGTH;
        head    = length - 1;
        dir     = user_dir = Dir::UP;

        for (uint8_t i = 0; i < length; ++i) {
            Point * const p = &tail[i];
            p->x = COLS >> 1;
            p->y = ROWS - (START_LENGTH + 2) + length - i;
            p->draw(COLOR);
        }

    }

    bool eatApple() {

        Point * const h = &tail[head];
        return h->x == apple.x && h->y == apple.y;

    }

    bool overlap(Point const * const p) {

        for (uint8_t i = p == &apple ? 0 : 3; i < length; ++i) {
            Point * const t = &tail[(head - i + MAX_LENGTH) % MAX_LENGTH];
            if (t->x == p->x && t->y == p->y) return true;
        }

        return false;

    }

    void extend() { length++; }

    void up()    { user_dir = Dir::UP;    }
    void right() { user_dir = Dir::RIGHT; }
    void down()  { user_dir = Dir::DOWN;  }
    void left()  { user_dir = Dir::LEFT;  }

    void update() {

        int8_t dx, dy;
        uint16_t const h = head;

        if (

            (user_dir == Dir::UP    && dir != Dir::DOWN)  ||
            (user_dir == Dir::RIGHT && dir != Dir::LEFT)  ||
            (user_dir == Dir::DOWN  && dir != Dir::UP)    ||
            (user_dir == Dir::LEFT  && dir != Dir::RIGHT)

        ) dir = user_dir;

        switch (dir) {
            case Dir::UP:    dx =  0; dy = -1; break;
            case Dir::RIGHT: dx =  1; dy =  0; break;
            case Dir::DOWN:  dx =  0; dy =  1; break;
            default:         dx = -1; dy =  0;
        }

        Point * const p = &tail[head];
        Point * const n = &tail[++head %= MAX_LENGTH];
        n->x = p->x + dx;
        n->y = p->y + dy;

        if (n->x + 1 > COLS || n->y + 1 > ROWS) {
            head = h;
            p->draw(0xffff);
            is_dead = true;
        }

        if (overlap(n)) {
            n->draw(0xffff);
            is_dead = true;
        }

    }

    void draw() {
        
        Point * const h = &tail[head];
        Point * const t = &tail[(head - length + MAX_LENGTH) % MAX_LENGTH];
        
        t->draw(0);
        h->draw(COLOR);

    }

} snake;

void spawnApple() {

    do {

        apple.x = random(COLS);
        apple.y = random(ROWS);

    } while (snake.overlap(&apple));

}

void displayScore(uint16_t const color) {

    espboy.tft.setTextColor(color, 0);
    espboy.tft.drawNumber(snake.score(), OX + COLS * SIZE - 2, OY + 2);

}

void reset() {

    snake.reset();
    spawnApple();

}

void waitForRestart() {

    displayScore(0xffe0);

    if (espboy.button.pressed(Button::ACT)) {
        espboy.tft.fillRect(OX - 1, OY - 1, COLS * SIZE + 2, ROWS * SIZE + 2, 0);
        reset();
    }

}

void update() {
 
    snake.update(); if (snake.is_dead) return;

    if (snake.eatApple()) {
        snake.extend();
        spawnApple();
    }

    displayScore(0x8410);
    apple.draw();
    snake.draw();

}

void setup() {

    espboy.begin();
    espboy.tft.setTextDatum(top_right);
    espboy.tft.drawRect(OX - 2, OY - 2, COLS * SIZE + 4, ROWS * SIZE + 4, 0x8410);
    reset();
}

void loop() {

    espboy.update();

    if (snake.is_dead) { waitForRestart(); return; }

         if (espboy.button.pressed(Button::LEFT))  snake.left();
    else if (espboy.button.pressed(Button::UP))    snake.up();
    else if (espboy.button.pressed(Button::RIGHT)) snake.right();
    else if (espboy.button.pressed(Button::DOWN))  snake.down();

    static uint32_t last = millis();
    uint32_t const now   = millis();

    if (now - last > 60) { update(); last = now; }

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