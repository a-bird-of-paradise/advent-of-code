%option 8bit yylineno noyywrap fast align

%{
#include "structs.h"
#include "13.tab.h"
%}

INT ([0-9]+)

%%

"["     |
"]"     |
,       {   return yytext[0];   }
\n      {   return ENDL;    }
{INT}   {   yylval.i = atoi(yytext); return INT;    }

