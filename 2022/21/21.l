%option 8bit yylineno noyywrap

%{
#include "21.tab.h"
#include "21.h"
%}

INT [-+]?[0-9]+
SYM [a-z]{4}

%%

"+" |
"-" |
"*" |
"/" |
":"     {   return *yytext; }

{SYM}   {   yylval.s = lookup(yytext);  return NAME;    }
{INT}   {   yylval.ll= atoll(yytext);   return NUMBER;  }

\n      {   return EOL;     }
[ \t]   {   /*  empty   */  }
