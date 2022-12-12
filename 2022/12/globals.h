#pragma once
#include "structs.h"

extern NODE **map;
extern char **textfile;

extern NODE *start, *end;
extern NODELIST to_visit, starts;

extern int ncols, rowcounter, nstarts;
extern unsigned *startdistances;