#ifndef SEQCLI_SEQFILE_H
#define SEQCLI_SEQFILE_H

#include <stdbool.h>

struct seqfile_t {
    char name[128];
    char musicFile[128];
};

typedef struct seqfile_t seqfile_t;

bool open_seqfile(const char *fp, seqfile_t *seqfile);

#endif//SEQCLI_SEQFILE_H
