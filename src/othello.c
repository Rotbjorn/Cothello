#include "othello.h"

typedef struct Cursor {
    int x, y;
} Cursor;

extern int WIDTH;
extern int HEIGHT;

static void cursor_up();
static void cursor_right();
static void cursor_down();
static void cursor_left();
static void to_cursor_pos();

static bool validate_input(const char *str, int *x, int *y);
static uint32_t validate_move(Othello *oth, int x, int y, Color c);
static bool validate_up(Othello *oth, int x, int y, Color c);
static bool validate_up_right(Othello *oth, int x, int y, Color c);
static bool validate_right(Othello *oth, int x, int y, Color c);
static bool validate_down_right(Othello *oth, int x, int y, Color c);
static bool validate_down(Othello *oth, int x, int y, Color c);
static bool validate_down_left(Othello *oth, int x, int y, Color c);
static bool validate_left(Othello *oth, int x, int y, Color c);
static bool validate_up_left(Othello *oth, int x, int y, Color c);

static void flip_pieces(Othello *oth, int valid, int x, int y, Color c);

static void flip_up(Othello *oth, int x, int y, Color c);
static void flip_up_right(Othello *oth, int x, int y, Color c);
static void flip_right(Othello *oth, int x, int y, Color c);
static void flip_down_right(Othello *oth, int x, int y, Color c);
static void flip_down(Othello *oth, int x, int y, Color c);
static void flip_down_left(Othello *oth, int x, int y, Color c);
static void flip_left(Othello *oth, int x, int y, Color c);
static void flip_up_left(Othello *oth, int x, int y, Color c);

static WINDOW *board_win;
static WINDOW *info_win;
static Cursor cursor;

void othello_init(Othello *oth) {
    // Initialize the board with empty slots
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            oth->board[x][y] = OTHELLO_EMPTY;
        }
    }

    // Initialize starter pieces
    oth->board[3][3] = OTHELLO_WHITE;
    oth->board[4][3] = OTHELLO_BLACK;
    oth->board[4][4] = OTHELLO_WHITE;
    oth->board[3][4] = OTHELLO_BLACK;

    int X = WIDTH / 2 - 8 * 2;
    int Y = HEIGHT / 3;

    clear();

    info_win = newwin(8, 40, 0, 0);
    keypad(info_win, true);
    box(info_win, 0, 0);
    wrefresh(info_win);

    board_win = newwin(17, 33, Y - 1, X - 1);
    keypad(board_win, true);
    box(board_win, 0, 0);

    mvwprintw(board_win, 0, 2 * 1, "A");
    mvwprintw(board_win, 0, 2 * 3, "B");
    mvwprintw(board_win, 0, 2 * 5, "C");
    mvwprintw(board_win, 0, 2 * 7, "D");
    mvwprintw(board_win, 0, 2 * 9, "E");
    mvwprintw(board_win, 0, 2 * 11, "F");
    mvwprintw(board_win, 0, 2 * 13, "G");
    mvwprintw(board_win, 0, 2 * 15, "H");

    mvwprintw(board_win, 1 + 2 * 0, 0, "1");
    mvwprintw(board_win, 1 + 2 * 1, 0, "2");
    mvwprintw(board_win, 1 + 2 * 2, 0, "3");
    mvwprintw(board_win, 1 + 2 * 3, 0, "4");
    mvwprintw(board_win, 1 + 2 * 4, 0, "5");
    mvwprintw(board_win, 1 + 2 * 5, 0, "6");
    mvwprintw(board_win, 1 + 2 * 6, 0, "7");
    mvwprintw(board_win, 1 + 2 * 7, 0, "8");

    move(1, 2);

    wrefresh(board_win);
}

void othello_draw(Othello *oth) {
    for (size_t y = 0; y < 8; y++) {
        for (size_t x = 0; x < 8; x++) {
            wchar_t c = u'·';
            switch (oth->board[x][y]) {
                case OTHELLO_BLACK:
                    c = 'B';
                    break;
                case OTHELLO_WHITE:
                    c = 'W';
                    break;
            }
            mvwprintw(board_win, 1 + y * 2, 2 + x * 4, "%lc", c);
        }
    }
    wrefresh(board_win);
    to_cursor_pos();
}

void othello_handle_input(Othello *oth, Color c) {
    bool done = false;
    while (!done) {
        int ch;
        ch = wgetch(board_win);
        switch (ch) {
            case KEY_UP:
                cursor_up();
                break;
            case KEY_RIGHT:
                cursor_right();
                break;
            case KEY_DOWN:
                cursor_down();
                break;
            case KEY_LEFT:
                cursor_left();
                break;
            case ' ': {
                uint32_t valid_move = validate_move(oth, cursor.x, cursor.y, c);

                if (!valid_move) {
                    break;
                }

                oth->board[cursor.x][cursor.y] = c;

                flip_pieces(oth, valid_move, cursor.x, cursor.y, c);

                done = true;
                break;
            }
        }
    }
    mvwprintw(info_win, 1, 1, "Player done!");
    wrefresh(info_win);
}

void othello_play_player(Othello *oth, Color c) {
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

        uint32_t valid_move = validate_move(oth, x, y, c);

        if (!valid_move) {
            continue;
        }

        oth->board[x][y] = c;

        flip_pieces(oth, valid_move, x, y, c);

        valid = true;
    }
}

void othello_play_ai(Othello *oth) {
}

static void cursor_up() {
    cursor.y--;
    if (cursor.y < 0)
        cursor.y = 0;
    else if (cursor.y > 7)
        cursor.y = 7;
    wmove(board_win, 1 + cursor.y * 2, 2 + cursor.x * 4);
}
static void cursor_right() {
    cursor.x++;
    if (cursor.x < 0)
        cursor.x = 0;
    else if (cursor.x > 7)
        cursor.x = 7;
    wmove(board_win, 1 + cursor.y * 2, 2 + cursor.x * 4);
}
static void cursor_down() {
    cursor.y++;
    if (cursor.y < 0)
        cursor.y = 0;
    else if (cursor.y > 7)
        cursor.y = 7;
    wmove(board_win, 1 + cursor.y * 2, 2 + cursor.x * 4);
}
static void cursor_left() {
    cursor.x--;
    if (cursor.x < 0)
        cursor.x = 0;
    else if (cursor.x > 7)
        cursor.x = 7;
    wmove(board_win, 1 + cursor.y * 2, 2 + cursor.x * 4);
}


static void to_cursor_pos() {
    wmove(board_win, 1 + cursor.y * 2, 2 + cursor.x * 4);
}


static bool validate_input(const char *str, int *x, int *y) {
    if (!(str[0] >= 'A' && str[0] <= 'H')) {
        // printf("Bad alpha: %c\n", str[0]);
        return false;
    }
    if (!(str[1] >= '1' && str[1] <= '9')) {
        // printf("Bad number: %c\n", str[1]);
        return false;
    }

    *x = str[0] - 'A';
    *y = str[1] - '1';

    return true;
}

static uint32_t validate_move(Othello *oth, int x, int y, Color c) {
    if (oth->board[x][y] != OTHELLO_EMPTY) {
        //printf("There's already a piece there!\n");
        return false;
    }
    Color opponent = !c;

    uint32_t valid = 0;

    valid = validate_up(oth, x, y, c);

    valid <<= 1;
    valid |= validate_up_right(oth, x, y, c);

    valid <<= 1;
    valid |= validate_right(oth, x, y, c);

    valid <<= 1;
    valid |= validate_down_right(oth, x, y, c);

    valid <<= 1;
    valid |= validate_down(oth, x, y, c);

    valid <<= 1;
    valid |= validate_down_left(oth, x, y, c);

    valid <<= 1;
    valid |= validate_left(oth, x, y, c);

    valid <<= 1;
    valid |= validate_up_left(oth, x, y, c);

    return valid;
}

static bool validate_up(Othello *oth, int x, int y, Color c) {
    int test_x = x;
    int test_y = y - 1;

    if (test_y < 0)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No up\n");
        return false;
    }
    while (test_y-- >= 0) {
        assert(test_y >= 0 && "Negative index");
        if (oth->board[test_x][test_y] == OTHELLO_EMPTY)
            return false;
        if (oth->board[test_x][test_y] == c) {
            return true;
        }
    }
    return false;
}

static bool validate_up_right(Othello *oth, int x, int y, Color c) {
    int test_x = x + 1;
    int test_y = y - 1;

    if (test_x > 7 || test_y < 0)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No up right\n");
        return false;
    }

    test_x++;
    test_y--;
    while (true) {
        if (test_x > 7 || test_y < 0) {
            return false;
        }
        if (oth->board[test_x++][test_y--] == c) {
            return true;
        }
    }
}

static bool validate_right(Othello *oth, int x, int y, Color c) {
    int test_x = x + 1;
    int test_y = y;

    if (test_x > 7)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No right\n");
        return false;
    }

    while (test_x++ >= 0) {
        assert(test_x >= 0 && "Negative index");
        if (oth->board[test_x][test_y] == OTHELLO_EMPTY)
            return false;
        if (oth->board[test_x][test_y] == c) {
            return true;
        }
    }
    return false;
}
static bool validate_down_right(Othello *oth, int x, int y, Color c) {
    int test_x = x + 1;
    int test_y = y + 1;

    if (test_x > 7 || test_y > 7)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No down right\n");
        return false;
    }

    test_x++;
    test_y++;
    while (true) {
        if (test_x > 7 || test_y > 7) {
            return false;
        }
        if (oth->board[test_x++][test_y++] == c) {
            return true;
        }
    }
}
static bool validate_down(Othello *oth, int x, int y, Color c) {
    int test_x = x;
    int test_y = y + 1;

    if (test_y > 7)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No down\n");
        return false;
    }
    while (test_y++ >= 0) {
        assert(test_y >= 0 && "Negative index");
        if (oth->board[test_x][test_y] == OTHELLO_EMPTY)
            return false;
        if (oth->board[test_x][test_y] == c) {
            return true;
        }
    }
    return false;
}
static bool validate_down_left(Othello *oth, int x, int y, Color c) {
    int test_x = x - 1;
    int test_y = y + 1;

    if (test_x < 0 || test_y > 7)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No down left\n");
        return false;
    }

    test_x--;
    test_y++;
    while (true) {
        if (test_x < 0 || test_y > 7) {
            return false;
        }
        if (oth->board[test_x--][test_y++] == c) {
            return true;
        }
    }
}
static bool validate_left(Othello *oth, int x, int y, Color c) {
    int test_x = x - 1;
    int test_y = y;

    if (test_x < 0)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No left\n");
        return false;
    }

    while (test_x-- >= 0) {
        assert(test_x >= 0 && "Negative index");
        if (oth->board[test_x][test_y] == OTHELLO_EMPTY)
            return false;
        if (oth->board[test_x][test_y] == c) {
            return true;
        }
    }
    return false;
}

static bool validate_up_left(Othello *oth, int x, int y, Color c) {
    int test_x = x - 1;
    int test_y = y - 1;

    if (test_x < 0 || test_y < 0)
        return false;

    Color opponent = !c;

    if (oth->board[test_x][test_y] != opponent) {
        //printf("No up left\n");
        return false;
    }

    test_x--;
    test_y--;
    while (true) {
        if (test_x < 0 || test_y < 0) {
            return false;
        }
        if (oth->board[test_x--][test_y--] == c) {
            return true;
        }
    }
}

static void flip_pieces(Othello *oth, int valid, int x, int y, Color c) {
    if (valid & 0b10000000) {
        //printf("Up\n");
        flip_up(oth, x, y, c);
    }
    if (valid & 0b01000000) {
        //printf("Up right\n");
        flip_up_right(oth, x, y, c);
    }
    if (valid & 0b00100000) {
        //printf("Right\n");
        flip_right(oth, x, y, c);
    }
    if (valid & 0b00010000) {
        //printf("Down right\n");
        flip_down_right(oth, x, y, c);
    }
    if (valid & 0b00001000) {
        //printf("Down\n");
        flip_down(oth, x, y, c);
    }
    if (valid & 0b00000100) {
        //printf("Down left\n");
        flip_down_left(oth, x, y, c);
    }
    if (valid & 0b00000010) {
        //printf("Left\n");
        flip_left(oth, x, y, c);
    }
    if (valid & 0b00000001) {
        //printf("Up left\n");
        flip_up_left(oth, x, y, c);
    }
}

static void flip_up(Othello *oth, int x, int y, Color c) {
    while (oth->board[x][--y] != c) {
        oth->board[x][y] = c;
    }
}

static void flip_up_right(Othello *oth, int x, int y, Color c) {
    while (oth->board[++x][--y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_right(Othello *oth, int x, int y, Color c) {
    while (oth->board[++x][y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_down_right(Othello *oth, int x, int y, Color c) {
    while (oth->board[++x][++y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_down(Othello *oth, int x, int y, Color c) {
    while (oth->board[x][++y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_down_left(Othello *oth, int x, int y, Color c) {
    while (oth->board[--x][++y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_left(Othello *oth, int x, int y, Color c) {
    while (oth->board[--x][y] != c) {
        oth->board[x][y] = c;
    }
}
static void flip_up_left(Othello *oth, int x, int y, Color c) {
    while (oth->board[--x][--y] != c) {
        oth->board[x][y] = c;
    }
}
