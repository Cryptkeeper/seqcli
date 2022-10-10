#ifndef SEQCLI_VIEWPORT_H
#define SEQCLI_VIEWPORT_H

#include <stdbool.h>

extern int gViewportRow;
extern int gViewportCol;

void get_viewport_limits(int *maxRow, int *maxCol);

bool move_viewport(int rd, int cd);

#endif//SEQCLI_VIEWPORT_H
