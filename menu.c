#include "menu.h"

#include <stddef.h>

#include <ncurses.h>

#include "screen.h"

static menu_t *gCurrentMenu;

static int gCurrentOpt;

static bool switch_opt(int d) {
    int nextOpt = gCurrentOpt + d;

    if (nextOpt < 0) {
        nextOpt = gCurrentMenu->size - 1;
    } else if (nextOpt >= gCurrentMenu->size) {
        nextOpt = 0;
    }

    const bool optMoved = gCurrentOpt != nextOpt;
    gCurrentOpt = nextOpt;

    return optMoved;
}

static bool handle_keypress(int ch, bool *interrupt) {
    const menu_opt_t *selectedOpt = &gCurrentMenu->opts[gCurrentOpt];

    // attempt to handle textbox input if activated
    if (selectedOpt->textbox != NULL && handle_textbox_input(selectedOpt->textbox, ch)) {
        return true;
    }

    switch (ch) {
        case KEY_DOWN:
            return switch_opt(1);

        case KEY_UP:
            return switch_opt(-1);

        case KEY_ENTER:
        case 10: {
            if (selectedOpt->open != NULL) {
                selectedOpt->open();
            }

            return selectedOpt->open != NULL;
        }

        default:
            return false;
    }
}

static void draw(void) {
    addstr(gCurrentMenu->title);
    addch('\n');

    for (int i = 0; i < gCurrentMenu->size; i++) {
        const menu_opt_t *opt = &gCurrentMenu->opts[i];

        addch(i + '0');
        addstr(". ");
        addstr(opt->name);

        if (opt->textbox != NULL) {
            addstr(": ");
            draw_textbox(opt->textbox);
        }

        if (i == gCurrentOpt) {
            addstr(" *");
        }

        addch('\n');
    }
}

static void clear_all_textboxes(menu_t *menu) {
    for (int i = 0; i < menu->size; i++) {
        menu_opt_t opt = menu->opts[i];

        if (opt.textbox != NULL) {
            clear_textbox(opt.textbox);
        }
    }
}

static void set_current_menu(menu_t *menu) {
    gCurrentMenu = menu;
    gCurrentOpt = 0;

    clear_all_textboxes(menu);
}

bool init_menuscreen(struct screen_t *screen, menu_t *menu) {
    set_current_menu(menu);

    // bind internal menu calls back to screen controller
    screen->title = menu->title;
    screen->keypressFn = handle_keypress;
    screen->drawFn = draw;

    return true;
}
