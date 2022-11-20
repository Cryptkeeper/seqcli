#include "alert.h"

#include <ncurses.h>

#include "colors.h"
#include "fmt.h"
#include "screen.h"

struct alert_t {
    const char *title;
    const char *msg;
    alert_callback_t callback;
};

static struct alert_t gCurrentAlert;

static bool handle_keypress(int ch, bool *interrupt) {
    if (ch == KEY_ENTER || ch == 10) {
        gCurrentAlert.callback();

        return true;
    } else {
        return false;
    }
}

static void draw(void) {
    fbreak();

    findent();
    addstr(gCurrentAlert.title);

    fnewline();
    findent();
    addstr(gCurrentAlert.msg);

    fnewline();
    findent();
    fcoloron(PAIR_DEFAULT);
    addstr("Hit [ENTER] to dismiss");
    fnocolor(PAIR_DEFAULT);

    fbreak();
}

bool init_alert(const char *title, const char *msg, alert_callback_t callback) {
    gCurrentAlert.title = title;
    gCurrentAlert.msg = msg;
    gCurrentAlert.callback = callback;

    // bind internal menu calls back to screen controller
    gScreen.keypressFn = handle_keypress;
    gScreen.drawFn = draw;

    return true;
}
