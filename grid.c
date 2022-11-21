#include "grid.h"

#define gDefaultRowCount 32
#define gDefaultColCount 128

static row_data_t gRowData[gDefaultRowCount];

static output_t gOutputs[gDefaultRowCount * gDefaultColCount];

grid_t gGrid = {
        .rowCount = gDefaultRowCount,
        .colCount = gDefaultColCount,
        .rowData = &gRowData[0],
        .outputs = &gOutputs[0],
};

static int get_output_index(int row, int col) {
    return col + (gDefaultColCount * row);
}

output_t *get_output(int row, int col) {
    const int idx = get_output_index(row, col);
    return &gGrid.outputs[idx];
}

bool set_output(int row, int col, uint8_t data) {
    output_t *output = get_output(row, col);
    const bool changed = output->data != data;
    output->data = data;
    return changed;
}

row_data_t *get_row_data(int row) {
    return &gGrid.rowData[row];
}
