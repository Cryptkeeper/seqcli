#include "colors.h"

#include <ncurses.h>

void init_color_pairs(void) {
    init_pair(PAIR_DEFAULT, COLOR_BLACK, COLOR_WHITE);
    init_pair(PAIR_INVERTED, COLOR_WHITE, COLOR_BLACK);
    init_pair(PAIR_HIGHLIGHT, COLOR_WHITE, COLOR_BLUE);
    init_pair(PAIR_ERROR, COLOR_WHITE, COLOR_RED);
    init_pair(PAIR_CURSOR, COLOR_WHITE, COLOR_YELLOW);
}
