%option noyywrap main 8bit fast align

%{
#include <string.h>
int start1, end1, start2, end2;
int paircount = 0, overlapcount = 0;
%}

NUM [0-9]+

%%
^{NUM}/-    {   start1 = atoi(yytext);      }
-{NUM}/,    {   end1 = atoi(yytext+1);      }
,{NUM}/-    {   start2 = atoi(yytext+1);    }
-{NUM}$     {   end2 = atoi(yytext+1);      }
\n          {   
    if( (start2 >= start1 && start2 <= end1 && end2 <= end1)
        || (start1 >= start2 && start1 <= end2 && end1 <= end2) ){
            paircount++;
        }

    if( (start1 <= start2 && end1 >= start2) ||
        (start2 <= start1 && end2 >= start1)) {
            overlapcount++;
        }
}
<<EOF>>     {   printf("%8d %8d\n", paircount, overlapcount); return 0;   }

%%