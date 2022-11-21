#ifndef SEQCLI_GRID_H
#define SEQCLI_GRID_H

#include <stdbool.h>
#include <stdint.h>

#define gLabelColWidth 20
#define gLabelRowHeight 1

typedef struct output_t {
    uint8_t data;
} output_t;

typedef struct row_data_t {
    char *name;
} row_data_t;

typedef struct grid_t {
    int colCount;
    int rowCount;
    row_data_t *rowData;
    output_t *outputs;
} grid_t;

extern grid_t gGrid;

output_t *get_output(int row, int col);

bool set_output(int row, int col, uint8_t data);

row_data_t *get_row_data(int row);

#endif//SEQCLI_GRID_H
