#ifndef SEQCLI_COLORS_H
#define SEQCLI_COLORS_H

typedef enum color_pair_t {
    PAIR_DEFAULT = 1,
    PAIR_INVERTED,
    PAIR_HIGHLIGHT,
    PAIR_ERROR,
    PAIR_CURSOR,
    PAIR_TYPING,
} color_pair_t;

void init_color_pairs(void);

#endif//SEQCLI_COLORS_H
