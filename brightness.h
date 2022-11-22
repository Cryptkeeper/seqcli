#ifndef SEQCLI_BRIGHTNESS_H
#define SEQCLI_BRIGHTNESS_H

#include <stdint.h>

#define gBrightnessLevelsCount 10

extern uint8_t gBrightnessLevels[gBrightnessLevelsCount];

unsigned int /* chtype */ get_brightness_icon(uint8_t brightness);

#endif//SEQCLI_BRIGHTNESS_H
