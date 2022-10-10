#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#include "brightness.h"
#include "colors.h"
#include "cursor.h"
#include "grid.h"
#include "keyboard.h"
#include "select.h"
#include "viewport.h"

static select_t gSelection;

static chtype get_icon(output_t *output) {
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
        addstr(gLabelBuffer);

        addch(' ');
        addch(get_row_icon(row));
        addch(ACS_VLINE);

        attroff(COLOR_PAIR(PAIR_INVERTED));

        for (int col = gViewportCol; col < maxCol; col++) {
            output_t *output = get_output(row, col);

            chtype icon_ch = get_icon(output);

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

        addch('\n');
    }

    static int redrawCounter = 0;

    addch('\n');
    addch('\n');
    redrawCounter++;
    printw("redraw: %d\n\nrow: %d\ncol: %d\nselecting: %d\n\nviewport: %d %d\n", redrawCounter, gCursorRow, gCursorCol, gSelection.active, gViewportRow, gViewportCol);
}

int main() {
    initscr();
    noecho();
    cbreak();

    curs_set(0);

    keypad(curscr, TRUE);

    start_color();

    init_color_pairs();

redrawGrid:
    clear();
    draw_grid();

    refresh();

    int ch;
    while (true) {
        ch = wgetch(curscr);

        bool interrupt = false;
        bool redraw = handle_keypress(ch, &interrupt, &gSelection);

        if (interrupt) goto exit;
        if (redraw) goto redrawGrid;
    }

exit:
    endwin();

    return 0;
}
