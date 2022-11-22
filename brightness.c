#include "brightness.h"

uint8_t gBrightnessLevels[gBrightnessLevelsCount] = {
        0,
        50,
        72,
        94,
        116,
        138,
        160,
        182,
        204,
        255,
};

static int get_closest_brightness(uint8_t brightness) {
    // identify the closest value in the levels array
    int diff = 0, idx = -1;

    for (int i = 0; i < gBrightnessLevelsCount; i++) {
        const uint8_t test = gBrightnessLevels[i];

        if (test == brightness) {
            return i;
        } else if (test > brightness) {
            const int localDiff = test - brightness;

            if (idx == -1 || localDiff < diff) {
                idx = i;
                diff = localDiff;
            }
        } else {
            continue;
        }
    }

    return idx;
}

unsigned int /* chtype */ get_brightness_icon(uint8_t brightness) {
    const int idx = get_closest_brightness(brightness);

    if (idx == 0) {
        return ' ';
    } else {
        return idx + '0';
    }
}
