#pragma once

#include "pch.h"

typedef enum Color {
    OTHELLO_BLACK, OTHELLO_WHITE, OTHELLO_EMPTY
} Color;

typedef struct Othello {
    uint8_t board[8][8];
} Othello;


void othello_draw(Othello *);
void othello_play_player(Othello *);
void othello_play_ai(Othello *);
