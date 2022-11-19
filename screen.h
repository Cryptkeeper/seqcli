#ifndef SEQCLI_SCREEN_H
#define SEQCLI_SCREEN_H

#include <stdbool.h>

typedef bool (*keypress_fn_t)(int ch, bool *interrupt);

typedef void (*draw_fn_t)(void);

struct screen_type_t {
    const char *title;
    keypress_fn_t keypressFn;
    draw_fn_t drawFn;
};

#endif//SEQCLI_SCREEN_H
