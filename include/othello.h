#pragma once

#include "pch.h"

typedef enum Color {
    OTHELLO_BLACK, OTHELLO_WHITE, OTHELLO_EMPTY
} Color;

typedef struct Othello {
    uint8_t board[8][8];
} Othello;


void othello_init(Othello *);
void othello_draw(Othello *);
void othello_handle_input(Othello *, Color c);
void othello_play_player(Othello *, Color c);
void othello_play_ai(Othello *);
