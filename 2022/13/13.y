%{
#include "structs.h"
#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yy_pairindex = 0;
int yy_indexcounter = 0;
%}

%union {    NODE *n;    int i;  }

%token ENDL
%token <i> INT
%type <n> list list_inner line list_item
%type <i> linepair linepairs
%type ending file

%destructor { free_node($$);  } <n>

%%

file:   linepairs                   {   printf("%8d\n",$1); part2();    }

linepairs: linepair                 {   $$ = $1;    }
    | linepairs linepair            {   $$ += $2;   };

linepair: line line ending          {   yy_pairindex++;
                                        $$ = compare($1,$2);
                                        yy_indexcounter += $$;  };

line: list ENDL {   add_node_to_list(&list_of_nodes,$1); $$ = $1;   };

list:   '[' list_inner ']'          {   $$ = new_node($2,NULL,LIST);};

list_inner:                         {   $$ = NULL;                  }
    |   list_item ',' list_inner    {   $1->right = $3; $$ = $1;    }
    |   list_item                   {   $$ = $1;                    };

list_item:
        INT     {   $$ = new_num($1);   }
    |   list    {   $$ = $1;            };

ending: ENDL | YYEOF;
