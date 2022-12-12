#include "globals.h"
NODE **map;
char **textfile;

NODE *start, *end;
NODELIST to_visit, starts;

int ncols, rowcounter=0, nstarts = 0;
