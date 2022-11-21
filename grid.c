#include "grid.h"

#include <stdlib.h>

grid_t *gGrid;

grid_t *alloc_grid(int rows, int cols) {
    const int count = rows * cols;

    output_t *outputs = malloc(count * sizeof(output_t));
    if (outputs == NULL) {
        goto abort;
    }

    row_data_t *rowData = malloc(rows * sizeof(row_data_t));
    if (rowData == NULL) {
        goto abort;
    }

    grid_t *grid = malloc(sizeof(grid_t));
    if (grid == NULL) {
        goto abort;
    }

    grid->colCount = cols;
    grid->rowCount = rows;
    grid->rowData = rowData;
    grid->outputs = outputs;

ok:
    return grid;

abort:
    dealloc_grid(grid);

    return NULL;
}

void dealloc_grid(grid_t *grid) {
    free(grid->outputs);
    free(grid->rowData);
    free(grid);
}

static int get_output_index(int row, int col) {
    return col + (gGrid->colCount * row);
}

output_t *get_output(int row, int col) {
    const int idx = get_output_index(row, col);
    return &gGrid->outputs[idx];
}

bool set_output(int row, int col, uint8_t data) {
    output_t *output = get_output(row, col);
    const bool changed = output->data != data;
    output->data = data;
    return changed;
}

row_data_t *get_row_data(int row) {
    return &gGrid->rowData[row];
}
