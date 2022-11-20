#ifndef SEQCLI_ALERT_H
#define SEQCLI_ALERT_H

#include <stdbool.h>

typedef void (*alert_callback_t)(void);

bool init_alert(const char *title, const char *msg, alert_callback_t callback);

#endif//SEQCLI_ALERT_H
