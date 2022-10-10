#include "grid.h"

static grid_t gSequence;

output_t *get_output(int row, int col) {
    return &gSequence[row][col];
}

bool set_output(int row, int col, uint8_t data) {
    output_t *output = get_output(row, col);
    bool changed = output->data != data;
    output->data = data;
    return changed;
}

static row_data_t gRowData[gRowCount];

row_data_t *get_row_data(int row) {
    return &gRowData[row];
}
