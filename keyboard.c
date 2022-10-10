#include "keyboard.h"

#include <ncurses.h>

#include "brightness.h"
#include "cursor.h"
#include "grid.h"
#include "select.h"
#include "viewport.h"

static bool modify_selection(int cd, uint8_t data, struct select_t *selection) {
    if (selection->active) {
        int minRow, maxRow;
        int minCol, maxCol;
        get_select_bounds(selection, &minRow, &maxRow, &minCol, &maxCol);

        bool anyOutputChanged = false;

        for (int row = minRow; row <= maxRow; row++) {
            for (int col = minCol; col <= maxCol; col++) {
                anyOutputChanged |= set_output(row, col, data);
            }
        }

        return anyOutputChanged;
    } else {
        bool outputChanged = set_output(gCursorRow, gCursorCol, data);
        bool cursorMoved = move_cursor(0, cd);

        return outputChanged || cursorMoved;
    }
}

bool handle_keypress(int ch, bool *interrupt, struct select_t *selection) {
    switch (ch) {
        case KEY_UP:
            return move_cursor(-1, 0);

        case KEY_DOWN:
            return move_cursor(+1, 0);

        case KEY_LEFT:
            return move_cursor(0, -1);

        case KEY_RIGHT:
        case ' ':
            return move_cursor(0, +1);

        case 'w':
            return move_viewport(-1, 0);

        case 's':
            return move_viewport(+1, 0);

        case 'a':
            return move_viewport(0, -1);

        case 'd':
            return move_viewport(0, +1);

        case KEY_BACKSPACE:
            return modify_selection(-1, 0, selection);

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            return modify_selection(+1, gBrightnessLevels[ch - '0'], selection);

        case 'f':
            return start_select(selection) || end_select(selection);

        case ',': {
            if (!end_select(selection)) return false;

            int minRow, maxRow;
            int minCol, maxCol;
            get_select_bounds(selection, &minRow, &maxRow, &minCol, &maxCol);

            int colStep = (int) (255.0F / ((float) (maxCol - minCol)));

            for (int y = minRow; y <= maxRow; y++) {
                for (int x = minCol; x <= maxCol; x++) {
                    set_output(y, x, (x - minCol) * colStep);
                }
            }

            return true;
        }

        case '.': {
            if (!end_select(selection)) return false;

            int minRow, maxRow;
            int minCol, maxCol;
            get_select_bounds(selection, &minRow, &maxRow, &minCol, &maxCol);

            int colStep = (int) (255.0F / ((float) (maxCol - minCol)));

            for (int y = minRow; y <= maxRow; y++) {
                for (int x = minCol; x <= maxCol; x++) {
                    set_output(y, x, (maxCol - x) * colStep);
                }
            }

            return true;
        }

        case 'q':
            *interrupt = true;
            return false;

        case KEY_RESIZE:
            // always redraw resize requests
            return true;

        default:
            return false;
    }
}
