#include "structs.h"
#include "13.tab.h"
#include <stdlib.h>

NODELIST list_of_nodes;

    int main(int argc, char **argv)
    {
        list_of_nodes.head = NULL;
        return yyparse();
    }