%option 8bit noyywrap nomain noyylineno  header-file="lex.yy.h"

%{
#include "structs.h"
int yy_column = 0, yy_line = -1;
extern BLIZZARD_LIST list;
%}

%%

#       {                   }
\.      {   yy_column++;    }

\n      {   yy_column = 0;  yy_line++;  }

<<EOF>> {   return   0;  }

"<"     {   add_blizzard(yy_line, yy_column, LEFT, &list);    yy_column++; }
">"     {   add_blizzard(yy_line, yy_column, RIGHT, &list);   yy_column++; }
[vV]    {   add_blizzard(yy_line, yy_column, DOWN, &list);    yy_column++; }
"^"     {   add_blizzard(yy_line, yy_column, UP, &list);      yy_column++; }
