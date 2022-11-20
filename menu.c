#include "menu.h"

#include <stddef.h>

#include <ncurses.h>

#include "colors.h"
#include "fmt.h"
#include "screen.h"

struct open_menu_t {
    menu_t *menu;
    int optIndex;
};

static struct open_menu_t gOpenMenu;

static inline const menu_opt_t *get_selected_menu_opt(void) {
    return &gOpenMenu.menu->opts[gOpenMenu.optIndex];
}

static bool switch_opt(int d) {
    const int maxOpt = gOpenMenu.menu->size;

    int nextOpt = gOpenMenu.optIndex + d;

    if (nextOpt < 0) {
        nextOpt = maxOpt - 1;
    } else if (nextOpt >= maxOpt) {
        nextOpt = 0;
    }

    const bool optMoved = gOpenMenu.optIndex != nextOpt;
    gOpenMenu.optIndex = nextOpt;

    return optMoved;
}

static bool menu_input_has_errors(menu_t *menu) {
    bool hasError = false;

    for (int i = 0; i < menu->size; i++) {
        const menu_opt_t *opt = &menu->opts[i];

        if (opt->textbox != NULL && opt->textbox->h == 0 && opt->textbox->input_required) {
            hasError = opt->textbox->error = true;
        }
    }

    return hasError;
}

static bool handle_keypress(int ch, bool *interrupt) {
    const menu_opt_t *selectedOpt = get_selected_menu_opt();

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
            if (selectedOpt->open != NULL && !menu_input_has_errors(gOpenMenu.menu)) {
                selectedOpt->open();
            }

            return selectedOpt->open != NULL;
        }

        default:
            return false;
    }
}

static void draw(void) {
    fbreak();

    findent();
    fcoloron(PAIR_HIGHLIGHT);
    addstr(gOpenMenu.menu->title);
    fnocolor(PAIR_HIGHLIGHT);

    fnewline();

    for (int i = 0; i < gOpenMenu.menu->size; i++) {
        const menu_opt_t *opt = &gOpenMenu.menu->opts[i];
        const bool isSelected = gOpenMenu.optIndex == i;

        if (isSelected) {
            fselected();
        } else {
            findent();
        }

        addstr(opt->name);

        if (opt->textbox != NULL) {
            addstr(": ");
            draw_textbox(opt->textbox);
        }

        for (int row = 0; row <= opt->rowpad; row++) {
            fbreak();
        }
    }
}

static void set_current_menu(menu_t *menu) {
    gOpenMenu.menu = menu;
    gOpenMenu.optIndex = 0;
}

bool init_menuscreen(menu_t *menu) {
    set_current_menu(menu);

    // bind internal menu calls back to screen controller
    gScreen.keypressFn = handle_keypress;
    gScreen.drawFn = draw;

    return true;
}
