#ifndef SEQCLI_TEXTBOX_H
#define SEQCLI_TEXTBOX_H

#include <stdbool.h>

struct textbox_t {
    bool is_typing;
    char input[256];
    int h;
};

void draw_textbox(const struct textbox_t *textbox);

bool handle_textbox_input(struct textbox_t *textbox, int ch);

void clear_textbox(struct textbox_t *textbox);

#endif//SEQCLI_TEXTBOX_H
