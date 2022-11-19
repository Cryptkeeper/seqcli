#ifndef SEQCLI_CURSOR_H
#define SEQCLI_CURSOR_H

#include <stdbool.h>
#include <stdint.h>

extern int gCursorRow;
extern int gCursorCol;

void move_cursor_into_viewport(void);

bool set_cursor(int row, int col);

bool move_cursor(int rd, int cd);

#endif//SEQCLI_CURSOR_H
