#include "select.h"

#include "cursor.h"


bool is_selected(const select_t *select, int row, int col) {
    if (!select->active) return false;

    int minRow, maxRow;
    int minCol, maxCol;
    get_select_bounds(select, &minRow, &maxRow, &minCol, &maxCol);

    return row >= minRow && row <= maxRow && col >= minCol && col <= maxCol;
}

bool start_select(select_t *select) {
    if (select->active) return false;
    select->active = true;

    select->row = gCursorRow;
    select->col = gCursorCol;

    return true;
}

bool end_select(select_t *select) {
    if (!select->active) return false;
    select->active = false;

    return true;
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void get_select_bounds(const select_t *select, int *minRow, int *maxRow, int *minCol, int *maxCol) {
    if (minRow) *minRow = min(select->row, gCursorRow);
    if (maxRow) *maxRow = max(select->row, gCursorRow);

    if (minCol) *minCol = min(select->col, gCursorCol);
    if (maxCol) *maxCol = max(select->col, gCursorCol);
}
