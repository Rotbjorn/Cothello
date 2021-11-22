#include "pch.h"
#include "othello.h"

int WIDTH = 0;
int HEIGHT = 0;


void screen_setup();
void screen_finish();
void screen_ui();
void start_player_game();
void start_ai_game();


int main() {
    setlocale(LC_CTYPE, "");
    printf(
            " 1) Spelare mot Spelare"       "\n"
            " 2) Spelare mot AI"            "\n"
            );
    int answer = 0;
    scanf("%d", &answer);

    if (answer == 1) {
        start_player_game();
    } else if (answer == 2) {
        start_ai_game();
    }

    return 0;
}

void screen_setup() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    getmaxyx(stdscr, HEIGHT, WIDTH);
    refresh();
}

void screen_finish() {
    endwin();
}

void start_ai_game() {
    screen_setup();
    Othello oth;

    othello_init(&oth);

    // For testing DIAGONAL
    oth.board[5][2] = OTHELLO_WHITE;

    bool finished = false;

    while(!finished) {
        othello_draw(&oth);
        othello_play_player(&oth, OTHELLO_WHITE);
        othello_play_ai(&oth);
    }

    screen_finish();
}

void start_player_game() {
    screen_setup();
    Othello oth;

    othello_init(&oth);

    bool finished = false;

    while(!finished) {
        othello_draw(&oth);
        othello_handle_input(&oth, OTHELLO_WHITE);
        othello_draw(&oth);
        othello_handle_input(&oth, OTHELLO_BLACK);
        /*
        othello_play_player(&oth, OTHELLO_WHITE);
        othello_draw(&oth);
        othello_play_player(&oth, OTHELLO_BLACK);
        */
    }
    screen_finish();
}
