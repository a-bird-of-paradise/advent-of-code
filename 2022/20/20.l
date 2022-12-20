%option 8bit noyywrap nomain fast align batch

%{
#include "structs.h"
%}

%%

[-+]?[0-9]+ {   add(atoi(yytext));          }
\n          {                               }
<<EOF>>     {   list.tail->next = list.head;
                list.head->prev = list.tail;
                return 0;                   }
