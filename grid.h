#ifndef SEQCLI_GRID_H
#define SEQCLI_GRID_H

#include <stdbool.h>
#include <stdint.h>

typedef struct output_t {
    uint8_t data;
} output_t;

#define gColCount 128
#define gRowCount 32

#define gLabelColWidth 20
#define gLabelRowHeight 1

typedef output_t row_t[gColCount];
typedef row_t grid_t[gRowCount];

output_t *get_output(int row, int col);

bool set_output(int row, int col, uint8_t data);

typedef struct row_data_t {
    char *name;
} row_data_t;

row_data_t *get_row_data(int row);

#endif//SEQCLI_GRID_H
