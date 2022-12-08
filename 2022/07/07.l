%option noyywrap main 8bit yylineno
%{
#include "funcs.h"
#include <stdlib.h>

unsigned long long yy_filesize = 0;
char *yy_spaceloc = 0;
int yy_position = 0;

%}

%x CD LS

%%

\$\ cd\     {   BEGIN CD;   }
<CD>{
\/\ *   {   if(!root_dir) init();   current_dir = root_dir; }
\.\.    {   current_dir = current_dir->parent;              }
.*      {   current_dir = find_child(current_dir,yytext);   }
\n      {   BEGIN INITIAL;  }
}

\$\ ls\n    {   BEGIN LS;   }
<LS>{
^dir\ .*$       {   add_child(current_dir,  yytext + 4, 0, true);  }
^[0-9]+\ .*$    {   yy_spaceloc = strchr(yytext,' ');
                    yy_position = yy_spaceloc - yytext;
                    yytext[yy_position] = 0;
                    yy_filesize = strtoull(yytext, NULL, 10);
                    add_child(current_dir, yy_spaceloc + 1, yy_filesize, false);
                    yytext[yy_position] = ' ';  }
\n              {   }
^\$             {   yyless(0);  BEGIN INITIAL;  }
}

<<EOF>>     {   set_node_sizes();   
                sum_below(100000);
                smallest_suitable_dir(70000000,30000000);
                return 0;   }
