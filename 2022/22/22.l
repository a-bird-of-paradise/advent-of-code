%option 8bit header-file="lex.yy.h" noyywrap nomain
INT [-+]?[0-9]+

%%

{INT}   {   return atoi(yytext);    }
L       {   return -1;              }
R       {   return -2;              }
\n      {                           }
<<EOF>> {   return 0;               }
