/**
 * ----------------------------------------------------------------------------
 * @file   9-2048.ino
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Remake of the famous game you probably know.
 * 
 * @note 2048 is a single-player sliding tile puzzle video game written by
 *       Italian web developer Gabriele Cirulli. The objective of the game is
 *       to slide numbered tiles on a grid to combine them to create a tile
 *       with the number 2048. However, one can continue to play the game after
 *       reaching the goal, creating tiles with larger numbers.
 * 
 *       2048 has been published on GitHub under MIT license in March 2014.
 * 
 * @see  https://github.com/gabrielecirulli/2048
 * 
 * @details You can play the original game online: https://play2048.co/
 * ----------------------------------------------------------------------------
 */

#include <ESPboy.h>

// ----------------------------------------------------------------------------
// Global constants
// ----------------------------------------------------------------------------

uint8_t constexpr TILE_SIZE = 27;

uint8_t const constexpr TILE[] PROGMEM = {

    // size: 27x27

    0x7f, 0xff, 0xff, 0xc0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xe0,
    0x7f, 0xff, 0xff, 0xc0

};

uint8_t constexpr POWER_OF_TWO_WIDTH      = 23;
uint8_t constexpr POWER_OF_TWO_HEIGHT     = 17;
uint8_t constexpr POWER_OF_TWO_FRAME_SIZE = ((POWER_OF_TWO_WIDTH + 1) >> 3) * POWER_OF_TWO_HEIGHT;

uint8_t const constexpr POWER_OF_TWO[] PROGMEM = {

    // size: 23x17

    /*      2 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x44, 0x00, 0x00, 0x04, 0x00, 0x00, 0x38, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*      4 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x28, 0x00, 0x00, 0x48, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*      8 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x44, 0x00, 0x00, 0x44, 0x00, 0x00, 0x38, 0x00, 0x00, 0x44, 0x00, 0x00, 0x44, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*     16 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x00, 0x01, 0x88, 0x00, 0x00, 0x90, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x91, 0x00, 0x00, 0x91, 0x00, 0x01, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*     32 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe7, 0x00, 0x00, 0x28, 0x80, 0x00, 0x40, 0x80, 0x00, 0xe7, 0x00, 0x00, 0x28, 0x00, 0x02, 0x28, 0x00, 0x01, 0xcf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*     64 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x00, 0x01, 0x03, 0x00, 0x02, 0x05, 0x00, 0x03, 0xc9, 0x00, 0x02, 0x2f, 0x80, 0x02, 0x21, 0x00, 0x01, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*    128 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x71, 0xc0, 0x0c, 0x8a, 0x20, 0x04, 0x0a, 0x20, 0x04, 0x71, 0xc0, 0x04, 0x82, 0x20, 0x04, 0x82, 0x20, 0x0e, 0xf9, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*    256 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x7c, 0x70, 0x11, 0x40, 0x80, 0x01, 0x79, 0x00, 0x0e, 0x05, 0xe0, 0x10, 0x05, 0x10, 0x10, 0x45, 0x10, 0x1f, 0x38, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*    512 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x91, 0xc0, 0x08, 0x32, 0x20, 0x0f, 0x10, 0x20, 0x00, 0x91, 0xc0, 0x00, 0x92, 0x00, 0x08, 0x92, 0x00, 0x07, 0x3b, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   1024 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x8e, 0x08, 0x64, 0x51, 0x18, 0x24, 0xc1, 0x28, 0x25, 0x4e, 0x48, 0x26, 0x50, 0x7c, 0x24, 0x50, 0x08, 0x73, 0x9f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   2048 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0xc1, 0x1c, 0x8a, 0x23, 0x22, 0x0a, 0x65, 0x22, 0x72, 0xa9, 0x1c, 0x83, 0x2f, 0xa2, 0x82, 0x21, 0x22, 0xf9, 0xc1, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   4096 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xc7, 0x0e, 0x32, 0x28, 0x90, 0x52, 0x68, 0xa0, 0x92, 0xa7, 0xbc, 0xfb, 0x20, 0xa2, 0x12, 0x21, 0x22, 0x11, 0xce, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   8192 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x8e, 0x38, 0x45, 0x91, 0x44, 0x44, 0x91, 0x04, 0x38, 0x8f, 0x38, 0x44, 0x81, 0x40, 0x44, 0x82, 0x40, 0x39, 0xdc, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*  16384 */ 0x00, 0x87, 0x00, 0x01, 0x88, 0x00, 0x00, 0x90, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x91, 0x00, 0x00, 0x91, 0x00, 0x01, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x38, 0x20, 0x01, 0x44, 0x60, 0x02, 0x44, 0xa0, 0x07, 0x39, 0x20, 0x01, 0x45, 0xf0, 0x11, 0x44, 0x20, 0x0e, 0x38, 0x20,
    /*  32768 */ 0x03, 0xe7, 0x00, 0x00, 0x28, 0x80, 0x00, 0x40, 0x80, 0x00, 0xe7, 0x00, 0x00, 0x28, 0x00, 0x02, 0x28, 0x00, 0x01, 0xcf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1c, 0xe0, 0x01, 0x21, 0x10, 0x01, 0x41, 0x10, 0x02, 0x78, 0xe0, 0x04, 0x45, 0x10, 0x08, 0x45, 0x10, 0x10, 0x38, 0xe0,
    /*  65536 */ 0x00, 0xef, 0x80, 0x01, 0x08, 0x00, 0x02, 0x0f, 0x00, 0x03, 0xc0, 0x80, 0x02, 0x20, 0x80, 0x02, 0x28, 0x80, 0x01, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x7c, 0x70, 0x10, 0x04, 0x80, 0x1e, 0x09, 0x00, 0x01, 0x1d, 0xe0, 0x01, 0x05, 0x10, 0x11, 0x45, 0x10, 0x0e, 0x38, 0xe0,
    /* 131072 */ 0x02, 0x7c, 0x80, 0x06, 0x05, 0x80, 0x02, 0x08, 0x80, 0x02, 0x1c, 0x80, 0x02, 0x04, 0x80, 0x02, 0x44, 0x80, 0x07, 0x39, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x7c, 0xe0, 0x11, 0x05, 0x10, 0x13, 0x04, 0x10, 0x15, 0x08, 0xe0, 0x19, 0x11, 0x00, 0x11, 0x21, 0x00, 0x0e, 0x41, 0xf0,

};

uint16_t constexpr BACKGROUND_COLOR = 0xb553; // hsl(30, 14, 66)
uint16_t constexpr DARK_COLOR       = 0x7b8c; // hsl(32,  8, 44)
uint16_t constexpr LIGHT_COLOR      = 0xffff; // white

uint16_t const constexpr TILE_COLOR[] PROGMEM = {

    0xce16, //      0 => hsl( 34,  18,  76)
    0xef19, //      2 => hsl( 40,  40,  86)
    0xe6b6, //      4 => hsl( 40,  46,  80)
    0xfef7, //      8 => hsl( 32,  80,  86)
    0xfdf3, //     16 => hsl( 20,  90,  80)
    0xfcf1, //     32 => hsl( 10,  90,  76)
    0xfc30, //     64 => hsl(  0, 100,  76)
    0xffb3, //    128 => hsl( 55, 100,  80)
    0xff66, //    256 => hsl( 55, 100,  60)
    0xfee6, //    512 => hsl( 50, 100,  60)
    0xfe66, //   1024 => hsl( 45, 100,  60)
    0xfdc6, //   2048 => hsl( 40, 100,  60)
    0x37f7, //   4096 => hsl(160, 100,  60)
    0x4796, //   8192 => hsl(160,  87,  60)
    0x4f34, //  16384 => hsl(154,  75,  60)
    0x5ed3, //  32768 => hsl(152,  62,  60)
    0x0f9e, //  65536 => hsl(180,  90,  50)
    0x1f3c  // 131072 => hsl(180,  80,  50)

};

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

LGFX_Sprite fb(&espboy.tft);

uint8_t  board[4][4];
uint8_t  free_tiles;
uint32_t score;
uint32_t higher;
uint32_t moves;
uint32_t wait_ms;
bool     slided;
bool     collapsed;

enum class State : uint8_t {

    start,
    play,
    loose,
    wait,
    game_over

} state;

// ----------------------------------------------------------------------------
// Global functions
// ----------------------------------------------------------------------------

void addNewTile() {

    uint8_t i, j, pow2;

    do { pow2 = board[i = random(4)][j = random(4)]; } while (pow2);

    board[i][j] = random(10) == 0 ? 2 : 1;
    free_tiles--;
    if (board[i][j] > higher) higher = board[i][j];

}

void slide(uint8_t const i) {

    uint8_t k;

    for (uint8_t j = 0; j < 3; ++j) {
        k = 1;
        if (board[i][j] == 0) {
            while (board[i][j+k] == 0 && j+k < 4) k++;
            if (j+k < 4) {
                board[i][j]   = board[i][j+k];
                board[i][j+k] = 0;
                slided        = true;
            }
        }
    }

}

void collapse(uint8_t const i) {

    for (uint8_t j = 0; j < 3; ++j) {
        if (board[i][j] && board[i][j] == board[i][j+1]) {
            board[i][j]++;
            board[i][j+1] = 0;
            if (board[i][j] == 11) espboy.pixel.rainbow(1000, 2);
            if (board[i][j] > higher) higher = board[i][j];
            score += 1 << board[i][j];
            free_tiles++;
            collapsed = true;
        }
    }

}

void tweak(uint8_t const transform) {

    uint8_t b[4][4]; memcpy(b, board, 16);

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            switch (transform) {
                case 0: board[i][j] = b[i][3-j]; break; // horizontal flip
                case 1: board[i][j] = b[j][3-i]; break; // rotate left
                case 2: board[i][j] = b[3-j][i]; break; // rotate right
                default:;
            }
        }
    }

}

void slideLeft() {

    for (uint8_t i = 0; i < 4; ++i) {
        slide(i);
        collapse(i);
        slide(i);
    }

}

void slideRight() { tweak(0); slideLeft(); tweak(0); }
void slideUp()    { tweak(1); slideLeft(); tweak(2); }
void slideDown()  { tweak(2); slideLeft(); tweak(1); }

bool isSqueezable() {

    uint8_t c = 0;

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            if (i < 3 && board[i][j] == board[i+1][j]) c++;
            if (j < 3 && board[i][j] == board[i][j+1]) c++;
        }
    }

    return c > 0;

}

void move(uint8_t const dir) {

    slided = collapsed = false;

    switch (dir) {
        case 0: slideLeft();  break;
        case 1: slideUp();    break;
        case 2: slideRight(); break;
        case 3: slideDown();  break;
        default:;
    }

    if (slided || collapsed) {
        moves++;
        addNewTile();
    }

    if (!free_tiles && !isSqueezable()) {
        state = State::loose;
    }

}

void start() {

    memset(board, 0, 16);
    free_tiles = 16;
    score = higher = moves = 0;

    espboy.pixel.reset();

    addNewTile();
    addNewTile();

    state = State::play;

}

void play() {

         if (espboy.button.pressed(Button::LEFT))  move(0);
    else if (espboy.button.pressed(Button::UP))    move(1);
    else if (espboy.button.pressed(Button::RIGHT)) move(2);
    else if (espboy.button.pressed(Button::DOWN))  move(3);

}

void loose() {

    espboy.pixel.breathe(Color::hsv2rgb(0), 250, 0);
    state   = State::wait;
    wait_ms = millis();

}

void wait()     { if (millis() - wait_ms < 2000) return;  state = State::game_over; }
void gameOver() { if (espboy.button.pressed(Button::ACT)) state = State::start;     }

void draw() {

    fb.clear(BACKGROUND_COLOR);

    uint8_t x, y, pow2;

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {

            pow2 = board[i][j];

            fb.drawBitmap(
                x = j * TILE_SIZE + ((j+1)<<2),
                y = i * TILE_SIZE + ((i+1)<<2),
                TILE,
                TILE_SIZE,
                TILE_SIZE,
                pgm_read_word(TILE_COLOR + pow2)
            );

            if (pow2) {

                fb.drawBitmap(
                    x + 2,
                    y + 5,
                    POWER_OF_TWO + (pow2 - 1) * POWER_OF_TWO_FRAME_SIZE,
                    POWER_OF_TWO_WIDTH,
                    POWER_OF_TWO_HEIGHT,
                    pow2 < 3 ? DARK_COLOR : LIGHT_COLOR
                );

            }

        }
    }

    if (state == State::game_over) {

        uint8_t const x = TFT_WIDTH >> 1;
        uint8_t const y = (TFT_HEIGHT - 47) >> 1;
        uint8_t const l = x - 40;
        uint8_t const r = x + 40;

        fb.fillRoundRect(l - 8, y - 12, 96, 71, 4, 0x5acb); // hsl(0, 0, 40)
        fb.setTextDatum(middle_center);
        fb.setTextColor(0xfb2c); // hsl(0, 100, 70)
        fb.drawString(F("GAME OVER"), x, y);

        fb.setTextDatum(top_left);
        fb.setTextColor(0xfe6c); // hsl(40, 100, 70)
        fb.drawString(F("High:"),  l, y + 16);
        fb.drawString(F("Score:"), l, y + 28);
        fb.drawString(F("Moves:"), l, y + 40);
        
        fb.setTextDatum(top_right);
        fb.setTextColor(0xffff); // white
        fb.drawNumber(1 << higher, r, y + 16);
        fb.drawNumber(score,       r, y + 28);
        fb.drawNumber(moves,       r, y + 40);

    }

    fb.pushSprite(0, 0);

}

// ----------------------------------------------------------------------------
// Main program
// ----------------------------------------------------------------------------

void setup() {

    espboy.begin();
    fb.createSprite(TFT_WIDTH, TFT_HEIGHT);
    state = State::start;

}

void loop() {

    espboy.update();

    switch (state) {

        case State::start:     start();    break;
        case State::play:      play();     break;
        case State::loose:     loose();    break;
        case State::wait:      wait();     break;
        case State::game_over: gameOver();

    }

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