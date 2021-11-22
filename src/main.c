#include "pch.h"
#include "othello.h"

int main() {

    Othello oth;

    for(int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            oth.board[x][y] = OTHELLO_EMPTY;
        }
    }

    // Initialize starter pieces
    oth.board[3][3] = OTHELLO_WHITE;
    oth.board[4][3] = OTHELLO_BLACK;
    oth.board[4][4] = OTHELLO_WHITE;
    oth.board[3][4] = OTHELLO_BLACK;


    bool finished = false;

    while(!finished) {
        othello_draw(&oth);
        othello_play_player(&oth);
        othello_play_ai(&oth);
    }

    return 0;
}

