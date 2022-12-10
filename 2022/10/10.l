%option 8bit main yylineno noyywrap fast align

%{
long long X = 1, strength;
void tick(void), printcrt(void);
long long cycle = 1;
char CRT[6*40];
%}

%%

noop            {   tick(); }
addx\ -?[0-9]+  {   tick(); tick(); X += atoll(yytext+5);   }
\n              {   }
<<EOF>>         {   printf("%10lld\n", strength);   printcrt(); return 0;   }
%%

void printcrt(void)
{
    for(int i = 0; i<6; i++) {
        for(int j = 0; j < 40; j++) printf("%c",CRT[40*i + j]);
        printf("\n");
    }
}

void tick(void)
{
    if(cycle == 1)
        for(int i = 0; i < 6 * 40; i++)
            CRT[i] = '.';

    if(cycle >= 20) 
        if( (cycle-20) % 40 == 0) 
            strength += cycle * X;

    if((cycle-1) % 40 == X || (cycle-1) % 40 == X + 1 || (cycle-1) % 40 == X - 1)
        CRT[cycle-1] = '#';

    cycle++;
}