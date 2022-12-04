%option noyywrap main 8bit fast align

%{
#include <ctype.h>
int items[52] = {0}, groupitems[52] = {0};
int getindex(const char x);
int priorities = 0, grouppriorities = 0, groupcounter = 1;
%}

%%

[A-Za-z]+   {
    for(int i = 0; i < yyleng / 2; ++i) {
        items[getindex(yytext[i])] |= 1;
        groupitems[getindex(yytext[i])] |= groupcounter;
    }
    for(int i = yyleng/2; i < yyleng; ++i) { 
        items[getindex(yytext[i])] |= 2; 
        groupitems[getindex(yytext[i])] |= groupcounter;
    }
    for(int i = 0; i < 52; i++) {
        if(items[i] == 3) {
            priorities += (i + 1);
        }
        items[i] = 0;
    }

    groupcounter *= 2;

    if(groupcounter == 8) {
        for(int i = 0; i < 52; i++){
            if(groupitems[i] == 7) {
                grouppriorities += (i + 1);
            }
            groupitems[i] = 0;
        }
        groupcounter = 1;
    }
}
\n          {   }
<<EOF>>     {   printf("%8d %8d\n", priorities, grouppriorities);    return 0;   }

%%

int getindex(const char x)
{
    if(x >= 'a' && x <= 'z') {
        return x - 'a';
    }
    else if(x >= 'A' && x <= 'Z') {
        return 26 + x - 'A';
    }
    else {
        printf("ERROR! %d", x);
        return x;
    }
}