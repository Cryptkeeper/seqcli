#include <stdbool.h>

#include <ncurses.h>

#include "colors.h"
#include "gridscreen.h"
#include "menu.h"
#include "screen.h"

static struct screen_t gCurrentScreen;

static void open_main_menu(void);

static struct textbox_t gMusicFileTextbox;

menu_opt_t gCreateSequenceMenuOpts[] = {
        {.name = "Select mp3 file", .textbox = &gMusicFileTextbox},
        {.name = "No music"},
        {.name = "Return to main menu", .open = open_main_menu},
};

menu_t gCreateSequenceMenu = {
        .title = "Create new sequence",
        .opts = gCreateSequenceMenuOpts,
        .size = 3,
};

static void open_create_sequence_menu(void) {
    init_menuscreen(&gCurrentScreen, &gCreateSequenceMenu);
}

menu_opt_t gMainMenuOpts[] = {
        {.name = "Create new sequence", .open = open_create_sequence_menu},
        {.name = "Open existing sequence"},
};

menu_t gMainMenu = {
        .title = "Main menu",
        .opts = gMainMenuOpts,
        .size = 2,
};

static void open_main_menu(void) {
    init_menuscreen(&gCurrentScreen, &gMainMenu);
}

int main() {
    /*if (!init_gridscreen(&gCurrentScreen, "testfile.seq")) {
        return 1;
    }*/

    open_main_menu();

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
