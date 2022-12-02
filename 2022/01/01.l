%option noyywrap main 8bit fast align

%{
#include <stdio.h>
#include <stdlib.h>
long current = 0, largest_1 = 0, largest_2 = 0, largest_3 = 0, top_3 = 0;
void largest(), result();
%}

%%

[0-9]+\n    {   current += atol(yytext);            }
^\n         {   largest();  current = 0;            }
<<EOF>>     {   largest();  result();   return 0;   }

%%

void largest()
{
    if(current > largest_3){
        if(current > largest_2) {
            if(current > largest_1) {
                largest_3 = largest_2;
                largest_2 = largest_3;
                largest_1 = current;
            } else {
                largest_3 = largest_2;
                largest_2 = current;
            }
        } else {
            largest_3 = current;
        }
    }
    top_3 = largest_1 + largest_2 + largest_3;
}

void result()
{
    printf("%8ld + %8ld + %8ld = %8ld\n", largest_1, largest_2, largest_3,  top_3); 
}