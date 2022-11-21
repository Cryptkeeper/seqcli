#include "viewport.h"

#include <ncurses.h>

#include "grid.h"

int gViewportRow = 0;
int gViewportCol = 0;

void get_viewport_limits(int *maxRow, int *maxCol) {
    int y, x;
    getmaxyx(curscr, y, x);

    y -= gLabelRowHeight;
    x -= gLabelColWidth;

    int rowLimit = gViewportRow + y;
    if (rowLimit > gGrid->rowCount) rowLimit = gGrid->rowCount;

    int colLimit = gViewportCol + x;
    if (colLimit > gGrid->colCount) colLimit = gGrid->colCount;

    *maxRow = rowLimit;
    *maxCol = colLimit;
}

static bool test_viewport_at(int row, int col) {
    if (row < 0 || row >= gGrid->rowCount) {
        return false;
    } else if (col < 0 || col >= gGrid->colCount) {
        return false;
    } else {
        return true;
    }
}

static bool set_viewport(int row, int col) {
    if (test_viewport_at(row, col)) {
        gViewportRow = row;
        gViewportCol = col;

        return true;
    } else {
        return false;
    }
}

bool move_cursor_into_viewport(void);// cursor.h

bool move_viewport(int rd, int cd) {
    bool viewportMoved = set_viewport(gViewportRow + rd, gViewportCol + cd);

    if (viewportMoved) {
        move_cursor_into_viewport();
    }

    return viewportMoved;
}
