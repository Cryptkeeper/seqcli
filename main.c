#include <stdbool.h>

#include <ncurses.h>

#include "colors.h"
#include "gridscreen.h"
#include "screen.h"

static struct screen_t gCurrentScreen;

int main() {
    if (!init_gridscreen(&gCurrentScreen, "testfile.seq")) {
        return 1;
    }

    initscr();
    noecho();
    cbreak();

    curs_set(0);

    keypad(curscr, TRUE);

    start_color();

    init_color_pairs();

redrawScreen:
    clear();
    gCurrentScreen.drawFn();

    refresh();

    int ch;
    while (true) {
        ch = wgetch(curscr);

        bool interrupt = false;
        bool redraw = gCurrentScreen.keypressFn(ch, &interrupt);

        if (interrupt) goto exit;
        if (redraw) goto redrawScreen;
    }

exit:
    endwin();

    return 0;
}
