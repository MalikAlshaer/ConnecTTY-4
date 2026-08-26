#include <stdio.h>
#include <string.h>
#define GRID_WIDTH 7
#define GRID_HEIGHT 6

char board[GRID_HEIGHT][GRID_WIDTH];

char *fg_black =    "\x1B[0;30m";
char *fg_red =      "\x1B[0;31m";
char *fg_green =    "\x1B[0;32m";
char *fg_yellow =   "\x1B[0;33m";
char *fg_blue =     "\x1B[0;34m";
char *fg_magenta =  "\x1B[0;35m";
char *fg_cyan =     "\x1B[0;36m";
char *fg_white =    "\x1B[0;37m";
char *reset =       "\x1B[0m";

char *bold =        "\x1B[1m";

#define blank_icon '.'
char player_0_icon = 'x';
char player_1_icon = 'o';

int player_turn = 0;
int game_run = 1;

void out_board() {
    printf("\x1B[2J\x1B[H"); // clear screen and home cursor (carriage return)
    printf("%sPLAYER %d TURN%s\n", bold, player_turn % 2 + 1, reset); // print PLAYER N TURN

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if      (board[y][x] == blank_icon)     printf("%s%c%s ", fg_white, board[y][x], reset);
            else if (board[y][x] == player_0_icon)  printf("%s%c%s ", fg_red,   board[y][x], reset);
            else if (board[y][x] == player_1_icon)  printf("%s%c%s ", fg_blue,  board[y][x], reset);
        }
        printf("\n");
    }

    for (int x = 1; x < GRID_WIDTH + 1; x++)
        printf("%d ", x);
    printf("\n");
}

void user_input() {
    int x, y, repeat = 0;

    do {
        if (repeat) {
            out_board();
            printf("Enter valid coordinates\n");
        }

        printf("Enter x coordinate: ");
        scanf("%d", &x);

        x -= 1; // convert to (1 .. GRID_WIDTH)

        for (int i = GRID_HEIGHT - 1; i >=  0; i--) {
            if (board[i][x] == blank_icon) {
                y = i;
                break;
            }
        }

        if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
            y = -1;
        }

        repeat = 1;
    } while (y == -1);


    if (player_turn % 2 == 0) {
        board[y][x] = player_0_icon;
    }
    else if (player_turn % 2 == 1) {
        board[y][x] = player_1_icon;
    }
}

int main() {
    memset(board, blank_icon, sizeof(board)); // clear board

    while (game_run) {
        out_board();
        user_input();
        player_turn++;
    }
    return 0;
}
