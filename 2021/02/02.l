%option noyywrap main 8bit fast align

%{
#include <stdio.h>
#include <stdlib.h>
long depth = 0, horizontal = 0, depth2 = 0, horizontal2 = 0, aim = 0;
long val = 0;
%}

%s FORWARD DOWN UP

%%

forward\            {   BEGIN FORWARD;  }
down\               {   BEGIN DOWN;  }
up\                 {   BEGIN UP;  }

<FORWARD>[0-9]+     {   val = atol(yytext); 
                            horizontal += val; 
                            horizontal2 += val; 
                            depth2 += (aim * val);  
                            BEGIN INITIAL;  }

<DOWN>[0-9]+        {   val = atol(yytext); 
                            depth += val; 
                            aim += val;   
                             BEGIN INITIAL;  }
                             
<UP>[0-9]+          {   val = atol(yytext); 
                            depth -= val; 
                            aim -= val;     
                            BEGIN INITIAL;  }

\n                  {   }

<<EOF>> {   printf("%8ld %8ld\n", depth * horizontal, depth2 * horizontal2); return 0; }

%%