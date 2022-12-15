%option 8bit fast align read noyywrap nomain header-file="lex.yy.h"

%{
#include "defs.h"
int xmax = INT16_MIN;
int ymax = INT16_MIN;
int xmin = INT16_MAX;
int ymin = INT16_MAX;
%}

INT [0-9]+

%%

{INT},{INT} {   
    char *comma = strchr(yytext,',');
    int comma_loc = comma-yytext;
    last_x = curr_x;    
    last_y = curr_y;
    *comma = 0;
    curr_x = atoi(yytext);
    *comma = ',';
    curr_y = atoi(yytext + comma_loc + 1);
    xmax = curr_x > xmax? curr_x : xmax; 
    ymax = curr_y > ymax? curr_y : ymax; 
    xmin = curr_x < xmin? curr_x : xmin; 
    ymin = curr_y < ymin? curr_y : ymin;
    if(pen_is_down) draw(); 
}
->          {   pen_is_up = false;  }
\n          {   pen_is_up = true;   }
\           {   /* do nothing */    }
.           {   printf("Unmatched garbage: %c\n",*yytext);   }
<<EOF>>     {   printf("%4d %4d %4d %4d\n",xmax,ymax,xmin,ymin);  
                escaped_y = ymax + 1;
                return  0;  }
