#include <termios.h>
#include <unistd.h>

struct termios orig_term;

void canonical() {
    struct termios new_term;

    tcgetattr(STDIN_FILENO, &orig_term);

    new_term = orig_term;

    new_term.c_lflag &= ~(ICANON | ECHO);

    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void non_canonical() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

