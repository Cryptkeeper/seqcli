#include "workdir.h"

#include <stdlib.h>
#include <string.h>

bool get_workdir(char *path, int n) {
    const char *dir = getenv("SEQCLI_WORKDIR");

    if (dir == NULL) {
        return false;
    }

    strncpy(path, dir, n);

    const size_t len = strlen(path);

    // strip final path separator
    // FIXME: expand multiplatform support
    if (path[len - 1] == '/') {
        path[len - 1] = '\0';
    }

    return true;
}
