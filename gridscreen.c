#include "gridscreen.h"

#include <stdlib.h>

#include <ncurses.h>

#include "brightness.h"
#include "colors.h"
#include "cursor.h"
#include "grid.h"
#include "select.h"
#include "viewport.h"

static select_t gSelection;

static bool modify_keyboard_selection(int cd, uint8_t data) {
    if (gSelection.active) {
        int minRow, maxRow;
        int minCol, maxCol;
        get_select_bounds(&gSelection, &minRow, &maxRow, &minCol, &maxCol);

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

static bool handle_keypress(int ch, bool *interrupt) {
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
            return modify_keyboard_selection(-1, 0);

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
            return modify_keyboard_selection(+1, gBrightnessLevels[ch - '0']);

        case 'f':
            return start_select(&gSelection) || end_select(&gSelection);

        case ',': {
            if (!end_select(&gSelection)) return false;

            int minRow, maxRow;
            int minCol, maxCol;
            get_select_bounds(&gSelection, &minRow, &maxRow, &minCol, &maxCol);

            int colStep = (int) (255.0F / ((float) (maxCol - minCol)));

            for (int y = minRow; y <= maxRow; y++) {
                for (int x = minCol; x <= maxCol; x++) {
                    set_output(y, x, (x - minCol) * colStep);
                }
            }

            return true;
        }

        case '.': {
            if (!end_select(&gSelection)) return false;

            int minRow, maxRow;
            int minCol, maxCol;
            get_select_bounds(&gSelection, &minRow, &maxRow, &minCol, &maxCol);

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

static chtype get_output_icon(output_t *output) {
    if (output->data == 0) {
        return ' ';
    }

    for (int i = 1; i < sizeof(gBrightnessLevels); i++) {
        if (gBrightnessLevels[i] == output->data) {
            return i + '0';
        }
    }

    return '?';
}

static chtype get_row_icon(int row) {
    if (row == gCursorRow) {
        return '*';
    } else if (gSelection.active) {
        int minRow, maxRow;
        get_select_bounds(&gSelection, &minRow, &maxRow, NULL, NULL);

        if (row >= minRow && row <= maxRow) {
            return '+';
        }
    }

    return ' ';
}

static void draw_grid(void) {
    attron(COLOR_PAIR(PAIR_DEFAULT));

    mvaddstr(0, 0, "OUTPUT LIST:      ");
    addch(ACS_VLINE);

    int maxRow, maxCol;
    get_viewport_limits(&maxRow, &maxCol);

    for (int col = gViewportCol; col < maxCol; col++) {
        if ((col + 1) % 4 == 0) {
            addch(ACS_VLINE);
        } else {
            addch(' ');
        }
    }

    addch('\n');

    attron(COLOR_PAIR(PAIR_DEFAULT));

    for (int row = gViewportRow; row < maxRow; row++) {
        row_data_t *rowData = get_row_data(row);

        attron(COLOR_PAIR(PAIR_INVERTED));

        static char gLabelBuffer[17];

        static char *gDefaultLabelFormat = "Output %d";
        static char *gLabelFormat = "%-16s";

        // format a default name
        if (rowData->name == NULL) {
            size_t n = snprintf(NULL, 0, gDefaultLabelFormat, row);
            rowData->name = malloc(n + 1);

            sprintf(rowData->name, gDefaultLabelFormat, row);
        }

        snprintf(gLabelBuffer, sizeof(gLabelBuffer), gLabelFormat, rowData->name);

        mvaddstr(gLabelRowHeight + (row - gViewportRow), 0, gLabelBuffer);

        addch(' ');
        addch(get_row_icon(row));
        addch(ACS_VLINE);

        attroff(COLOR_PAIR(PAIR_INVERTED));

        for (int col = gViewportCol; col < maxCol; col++) {
            output_t *output = get_output(row, col);

            chtype icon_ch = get_output_icon(output);

            // override color when highlighted by cursor
            color_pair_t colorPair = PAIR_INVERTED;

            if (row == gCursorRow && col == gCursorCol) {
                colorPair = PAIR_CURSOR;
            } else if (is_selected(&gSelection, row, col)) {
                colorPair = PAIR_HIGHLIGHT;
            }

            attron(COLOR_PAIR(colorPair));
            addch(icon_ch);
            attroff(COLOR_PAIR(colorPair));
        }
    }

    static int redrawCounter = 0;

    addch('\n');
    addch('\n');
    redrawCounter++;
    printw("redraw count: %d\n"
           "cursor: %d %d\n"
           "is selecting: %d\n"
           "viewport: %d %d\n"
           "limits: %d %d\n",
           redrawCounter, gCursorRow, gCursorCol, gSelection.active, gViewportRow, gViewportCol, maxRow, maxCol);
}

bool init_gridscreen(struct screen_type_t *screen, const char *filepath) {
    screen->title = filepath;
    screen->keypressFn = handle_keypress;
    screen->drawFn = draw_grid;

    return true;
}
