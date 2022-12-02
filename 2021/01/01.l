%option noyywrap main 8bit fast align

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

long current = 0, previous = 0, increased = 0;
long pprevious = 0, previous_sum3 = 0, current_sum3 = LONG_MAX, increased_sum3 = 0;

bool first = true, second = false;
%}

%%

[0-9]+  {   

    if(first) {
        current = atol(yytext); 
        first = false;
        second = true;
    } else if(second) {
        second = false;
        previous = current;
        current = atol(yytext);
        if(current > previous) increased++;
    } else {
        pprevious = previous;
        previous = current;
        current = atol(yytext);

        previous_sum3 = current_sum3;
        current_sum3 = current + previous + pprevious; 
        if(current > previous) increased++;
        if(current_sum3 > previous_sum3) increased_sum3++;
    }
}
\n      {   }
<<EOF>> {   printf("%8ld %8ld\n",increased,increased_sum3);  return 0;   }
