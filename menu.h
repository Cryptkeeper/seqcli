#ifndef SEQCLI_MENU_H
#define SEQCLI_MENU_H

#include <stdbool.h>

#include "textbox.h"

struct screen_t;

typedef struct menu_opt_t {
    char *name;
    struct textbox_t *textbox;
    void (*open)(void);
} menu_opt_t;

typedef struct menu_t {
    char *title;
    const menu_opt_t *opts;
    int size;
} menu_t;

bool init_menuscreen(struct screen_t *screen, menu_t *menu);

#endif//SEQCLI_MENU_H
