#ifndef SEQCLI_KEYBOARD_H
#define SEQCLI_KEYBOARD_H

#include <stdbool.h>

struct select_t;

bool handle_keypress(int ch, bool *interrupt, struct select_t *selection);

#endif//SEQCLI_KEYBOARD_H
