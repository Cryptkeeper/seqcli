#ifndef SEQCLI_FMT_H
#define SEQCLI_FMT_H

#define findent() addstr("   ")

#define fspace() addch(' ')

#define fselected()                \
    fspace();                      \
    fcoloron(PAIR_MENU_SELECTION); \
    addch('-');                    \
    fnocolor(PAIR_MENU_SELECTION); \
    fspace();

#define fnewline() addstr("\n\n")

#define fbreak() addstr("\n")

#define fcoloron(e) attron(COLOR_PAIR(e))

#define fnocolor(e) attroff(COLOR_PAIR(e))

#endif//SEQCLI_FMT_H
