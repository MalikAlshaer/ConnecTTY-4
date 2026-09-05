#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "canonicalize.h"

#define GRID_WIDTH 7
#define GRID_HEIGHT 6


#define KEY_RIGHT   "\x1B[C"
#define KEY_LEFT    "\x1B[D"
#define KEY_ENTER   "\n"
#define KEY_Q       "q"

#define FG_BLACK    "\x1B[30m"
#define FG_RED      "\x1B[31m"
#define FG_GREEN    "\x1B[32m"
#define FG_YELLOW   "\x1B[33m"
#define FG_BLUE     "\x1B[34m"
#define FG_MAGENTA  "\x1B[35m"
#define FG_CYAN     "\x1B[36m"
#define FG_WHITE    "\x1B[37m"
#define RESET       "\x1B[0m"
#define BOLD        "\x1B[1m"

#define CLEAR       "\x1B[2J\x1B[H"

typedef struct {
    char board[GRID_HEIGHT][GRID_WIDTH];
    int cursor_x;
    int turn;
    int running;
} Game;

char blank_icon     = '.';
char player_0_icon  = 'x';
char player_1_icon  = 'o';


void print_board(Game *game) {
    printf("%s", CLEAR); // clear screen and home cursor (carriage return)
    printf("%sPLAYER %d TURN%s\n", BOLD, game->turn % 2 + 1, RESET); // print PLAYER N (1, 2) TURN

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if      (x == game->cursor_x)                 printf("%s", BOLD);
            if      (game->board[y][x] == blank_icon)     printf("%s%c%s ", FG_WHITE, game->board[y][x], RESET);
            else if (game->board[y][x] == player_0_icon)  printf("%s%c%s ", FG_RED,   game->board[y][x], RESET);
            else if (game->board[y][x] == player_1_icon)  printf("%s%c%s ", FG_BLUE,  game->board[y][x], RESET);
            printf("%s", RESET);
        }
        printf("\n");
    }
}

void user_input(Game *game) {
    int y;
    char buf[5];

    memset(buf, 0, sizeof(buf));
    read(STDIN_FILENO, buf, sizeof(buf));

    if (strncmp(buf, KEY_RIGHT, strlen(KEY_RIGHT)) == 0 && game->cursor_x < GRID_WIDTH - 1)
        game->cursor_x += 1;

    else if (strncmp(buf, KEY_LEFT, strlen(KEY_LEFT)) == 0 && game->cursor_x > 0)
        game->cursor_x -= 1;

    else if (strncmp(buf, KEY_Q, strlen(KEY_Q)) == 0)
        game->running = 0;

    else if (strncmp(buf, KEY_ENTER, strlen(KEY_ENTER)) == 0) {
        // place player's token if there is space
        for (int i = GRID_HEIGHT - 1; i >=  0; i--) {
            if (game->board[i][game->cursor_x] == blank_icon) {
                y = i;

                if      (game->turn % 2 == 0) game->board[y][game->cursor_x] = player_0_icon;
                else if (game->turn % 2 == 1) game->board[y][game->cursor_x] = player_1_icon;

                game->cursor_x = 0;
                game->turn += 1;
                break;
            }
        }
    }
}

int check_win(Game *game) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (x <= GRID_WIDTH - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y][x + 1] && game->board[y][x + 1] == game->board[y][x + 2] && game->board[y][x + 2] == game->board[y][x + 3] || y <= GRID_HEIGHT - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x] && game->board[y + 1][x] == game->board[y + 2][x] && game->board[y + 2][x] == game->board[y + 3][x] || y <= GRID_HEIGHT - 4 && x <= GRID_WIDTH - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x + 1] && game->board[y + 1][x + 1] == game->board[y + 2][x + 2] && game->board[y + 2][x + 2] == game->board[y + 3][x + 3] || y <= GRID_HEIGHT - 4 && x >= 3 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x - 1] &&game->board[y + 1][x - 1] == game->board[y + 2][x - 2] &&game->board[y + 2][x - 2] == game->board[y + 3][x - 3]) {
                game->running = 0;
            }

            // a slightly more human approach follows

            // // check horizontal
            // if (x <= GRID_WIDTH - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y][x + 1] && game->board[y][x + 1] == game->board[y][x + 2] && game->board[y][x + 2] == game->board[y][x + 3]) {
            //     game->running = 0;
            // }

            // // check vertical
            // if (y <= GRID_HEIGHT - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x] && game->board[y + 1][x] == game->board[y + 2][x] && game->board[y + 2][x] == game->board[y + 3][x]) {
            //     game->running = 0;
            // }

            // // check diagonal right
            // if (y <= GRID_HEIGHT - 4 && x <= GRID_WIDTH - 4 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x + 1] && game->board[y + 1][x + 1] == game->board[y + 2][x + 2] && game->board[y + 2][x + 2] == game->board[y + 3][x + 3]) {
            //     game->running = 0;
            // }

            // // check diagonal left
            // if (y <= GRID_HEIGHT - 4 && x >= 3 && game->board[y][x] != blank_icon && game->board[y][x] == game->board[y + 1][x - 1] &&game->board[y + 1][x - 1] == game->board[y + 2][x - 2] &&game->board[y + 2][x - 2] == game->board[y + 3][x - 3]) {
            //     game->running = 0;
            // }

        }
    }
    return 0;
}

int main() {
    printf("\x1B[?25l"); // make cursor invisible

    int player_turn = 0;

    Game game = {.cursor_x = 0, .turn = 0, .running = 1};

    canonical(); // put terminal in canonical mode

    memset(game.board, blank_icon, sizeof(game.board)); // clear board

    print_board(&game);
    while (game.running) {
        user_input(&game);
        check_win(&game);
        print_board(&game);
    }

    // cleanup
    printf("\x1b[?25h"); // cursor visible
    non_canonical(); // terminal is now non_canonical
    return 0;
}
