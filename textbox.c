#include "textbox.h"

#include <ncurses.h>

#include <string.h>

#include "colors.h"

void draw_textbox(const struct textbox_t *textbox) {
    color_pair_t activePair = textbox->is_typing ? PAIR_TYPING : PAIR_INVERTED;

    attron(COLOR_PAIR(activePair));

    if (textbox->h == 0) {
        addstr("(empty)");
    } else {
        addstr(textbox->input);
    }

    attroff(COLOR_PAIR(activePair));
}

static inline bool is_printable_char(int ch) {
    return ch >= 32 && ch <= 126;
}

bool handle_textbox_input(struct textbox_t *textbox, int ch) {
    if (ch == KEY_ENTER || ch == 10) {
        textbox->is_typing = !textbox->is_typing;

        return true;
    }

    if (!textbox->is_typing) {
        return false;
    }

    switch (ch) {
        case KEY_BACKSPACE:
        case 127:
            if (textbox->h > 0) {
                textbox->input[textbox->h - 1] = '\0';
                textbox->h--;
            }

            return true;

        default: {
            if (is_printable_char(ch)) {
                if (textbox->h < sizeof(textbox->input)) {
                    // downcast bounds checking is protected by #is_printable_char
                    textbox->input[textbox->h++] = (char) ch;
                }
            }

            // capture ALL input when typing
            // prevents unhandled passing downstream
            return true;
        }
    }
}

void clear_textbox(struct textbox_t *textbox) {
    memset(textbox->input, 0, sizeof(textbox->input));

    textbox->h = 0;
    textbox->is_typing = false;
}
