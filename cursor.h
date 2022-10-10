#ifndef SEQCLI_CURSOR_H
#define SEQCLI_CURSOR_H

#include <stdbool.h>
#include <stdint.h>

extern int gCursorRow;
extern int gCursorCol;

bool set_cursor(int row, int col);

bool move_cursor(int rd, int cd);

bool jump_cursor(int rd, int cd);

bool test_cursor(int rd, int cd);

#endif//SEQCLI_CURSOR_H
