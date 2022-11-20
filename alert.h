#ifndef SEQCLI_ALERT_H
#define SEQCLI_ALERT_H

#include <stdbool.h>

#include "menu.h"

bool init_alert(const char *title, const char *msg, menu_callback_t callback);

#endif//SEQCLI_ALERT_H
