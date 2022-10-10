#ifndef SEQCLI_SELECT_H
#define SEQCLI_SELECT_H

#include <stdbool.h>

typedef struct select_t {
    bool active;
    int row;
    int col;
} select_t;

bool is_selected(const select_t *select, int row, int col);

bool start_select(select_t *select);

bool end_select(select_t *select);

void get_select_bounds(const select_t *select, int *minRow, int *maxRow, int *minCol, int *maxCol);

#endif//SEQCLI_SELECT_H
