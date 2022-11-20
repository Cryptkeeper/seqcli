#ifndef SEQCLI_MENU_H
#define SEQCLI_MENU_H

#include <stdbool.h>

#include "textbox.h"

typedef void (*menu_callback_t)(void);

typedef struct menu_opt_t {
    char *name;
    struct textbox_t *textbox;
    menu_callback_t open;
    int rowpad;
} menu_opt_t;

typedef struct menu_t {
    char *title;
    const menu_opt_t *opts;
    int size;
} menu_t;

bool init_menuscreen(menu_t *menu);

bool menu_input_has_errors(menu_t *menu);

#endif//SEQCLI_MENU_H
