#include <stdbool.h>

#include <ncurses.h>

#include "alert.h"
#include "colors.h"
#include "gridscreen.h"
#include "menu.h"
#include "screen.h"

static void open_main_menu(void);

static void create_sequence(void);

static struct textbox_t gSequenceNameTextbox;
static struct textbox_t gMusicFileTextbox;

menu_opt_t gCreateSequenceMenuOpts[] = {
        {.name = "Sequence name", .textbox = &gSequenceNameTextbox},
        {.name = "Music filepath", .textbox = &gMusicFileTextbox, .rowpad = 1},
        {.name = "Create sequence", .open = create_sequence, .rowpad = 2},
        {.name = "Return to main menu", .open = open_main_menu},
};

menu_t gCreateSequenceMenu = {
        .title = "Create new sequence",
        .opts = gCreateSequenceMenuOpts,
        .size = 4,
};

static void open_create_sequence_menu(void) {
    init_menuscreen(&gCreateSequenceMenu);
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
    init_menuscreen(&gMainMenu);
}

static void create_sequence(void) {
    char msg[1024];
    snprintf(msg, sizeof(msg), "Your sequence file has been created: %s", gSequenceNameTextbox.input);

    init_alert("Sequence created!", msg, open_main_menu);
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
