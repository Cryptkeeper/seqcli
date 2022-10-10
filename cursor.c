#include "cursor.h"

#include "grid.h"
#include "viewport.h"

int gCursorRow = 0;
int gCursorCol = 0;

static bool is_cursor_in_viewport(void) {
    int maxRow, maxCol;
    get_viewport_limits(&maxRow, &maxCol);

    return gCursorRow >= gViewportRow && gCursorRow <= maxRow && gCursorCol >= gViewportCol && gCursorCol <= maxCol;
}

static bool test_cursor_at(int row, int col) {
    if (row < 0 || row >= gRowCount) {
        return false;
    } else if (col < 0 || col >= gColCount) {
        return false;
    } else {
        return true;
    }
}

bool set_cursor(int row, int col) {
    if (test_cursor_at(row, col)) {
        gCursorRow = row;
        gCursorCol = col;

        return true;
    } else {
        return false;
    }
}

static bool wrap_cursor(int rd) {
    if (rd == 0) return false;

    if (rd < 0) {
        gCursorRow = gRowCount - 1;
    } else /* if (rd > 0) */ {
        gCursorRow = 0;
    }

    return true;
}

bool move_cursor(int rd, int cd) {
    const bool cursorMoved = set_cursor(gCursorRow + rd, gCursorCol + cd);

    if (!cursorMoved) {
        // unable to move cursor, likely at a grid limit
        // attempt to wrap the cursor around
        if (!wrap_cursor(rd)) return false;
    }

    // shift viewport to ensure the cursor is always visible
    if (!is_cursor_in_viewport()) {
        move_viewport(rd, cd);
    }

    return true;
}

#define clamp(c, m, x) (c < m ? m : (c > x ? x : c))

bool jump_cursor(int rd, int cd) {
    int row = clamp(gCursorRow + rd, 0, gRowCount);
    int col = clamp(gCursorCol + cd, 0, gColCount);

    if (row == gCursorRow && col == gCursorCol) {
        return false;
    } else {
        return set_cursor(row, col);
    }
}

bool test_cursor(int rd, int cd) {
    return test_cursor_at(gCursorRow + rd, gCursorCol + cd);
}
