#include "othello.h"

static bool validate_input(const char* str, int* x, int* y);
static bool validate_move(Othello* oth, int x, int y, Color c);
static bool validate_up(Othello* oth, int x, int y, Color c);
static bool validate_right(Othello* oth, int x, int y, Color c);
static bool validate_down(Othello* oth, int x, int y, Color c);
static bool validate_left(Othello* oth, int x, int y, Color c);


void othello_draw(Othello *oth) {
    for (size_t y = 0; y < 8; y++) {
        for (size_t x = 0; x < 8; x++) {
            char c = '.';
            switch(oth->board[x][y]) {
                case OTHELLO_BLACK:
                    c = 'B';
                    break;
                case OTHELLO_WHITE:
                    c = 'W';
                    break;
            }
            printf("%c ", c);
        }
        printf("\n");
    }
}


void othello_play_player(Othello * oth) {
    char input[3];

    bool valid = false;

    int x, y;
    while (!valid) {

        scanf("%s", input);
        input[2] = 0;

        printf("Input: %s\n", input);
        bool valid_input = validate_input(input, &x, &y);

        if (!valid_input) {
            printf("Invalid input! [A-H][1-8]\n");
            continue;
        }

        // Input is valid, validate move
        

        bool valid_move = validate_move(oth, x, y, OTHELLO_WHITE);


        if (!valid_move) {
            continue;
        }

        oth->board[x][y] = OTHELLO_WHITE;
    }
}


void othello_play_ai(Othello * oth) {

}



static bool validate_input(const char* str, int* x, int* y) {
    if (!(str[0] >= 'A' && str[0] <= 'H')) {
        //printf("Bad alpha: %c\n", str[0]);
        return false;
    }
    if (!(str[1] >= '1' && str[1] <= '9')) {
        //printf("Bad number: %c\n", str[1]);
        return false;
    }

    *x = str[0] - 'A';
    *y = str[1] - '1';

    return true;
}


static bool validate_move(Othello* oth, int x, int y, Color c) {
    if (oth->board[x][y] != OTHELLO_EMPTY) {
        printf("There's already a piece there!\n");
        return false;
    }
    Color opponent = !c;

    int valid = 0;

    valid = validate_up(oth, x, y, c) << 1;
    valid <<= 1;

    valid = validate_right(oth, x, y, c);
    valid <<= 1;

    valid = validate_down(oth, x, y, c);
    valid <<= 1;

    valid = validate_left(oth, x, y, c);
    valid <<= 1;


    if (valid > 0) {
        printf("There is some valid move\n");
        return true;
    }


    return false;
}


static bool validate_up(Othello* oth, int x, int y, Color c) {
    int test_x = x;
    int test_y = y - 1;

    Color opponent = !c;

    if (oth->board[test_x][test_y] == opponent) {
        return false;
    }

    while (--test_y != 0) {
        if (oth->board[test_x][test_y] == OTHELLO_EMPTY) return false;
        if (oth->board[test_x][test_y] == c) {
            return true;
        }
    }
    return false;
}
static bool validate_right(Othello* oth, int x, int y, Color c) {
    return false;
}
static bool validate_down(Othello* oth, int x, int y, Color c) {
    return false;
}
static bool validate_left(Othello* oth, int x, int y, Color c) {
    return false;
}
