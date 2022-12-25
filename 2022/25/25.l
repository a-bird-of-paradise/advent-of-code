%option 8bit noyywrap main

%{
long long cval = 0, tval = 0;
int col = 0, part1[100] = {0};
%}

%%

=       {   cval *= 5;  cval +=-2;              }
-       {   cval *= 5;  cval +=-1;              }
[0-2]   {   cval *= 5;  cval += atoi(yytext);   }
\n      {   tval += cval; cval = 0;             }
<<EOF>> {   

while(tval > 0) {part1[col++]=tval % 5; tval /= 5;}

for(int i = 0; i < col; i++)
    if(part1[i]>=3) {part1[i] -= 5; part1[i+1] += 1;}

for(int i = col; i >= 0; i--)
{
    if(i == col && part1[i]==0) continue;
    switch(part1[i]){
        case -2:    printf("="); break;
        case -1:    printf("-"); break;
        default:    printf("%d",part1[i]);
    }
}
printf("\n");
return 0;   
}
