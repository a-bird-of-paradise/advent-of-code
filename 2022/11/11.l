%option 8bit main yylineno noyywrap fast align
%option header-file="lex.yy.h"

%{
#include "monkey.h"
MONKEY monkeys[8], *this_monkey;
int divisor, operand, truthy, falsy, modulo =1;
OP oper;
ITEM *itemlist;
unsigned long long top_2(void);
%}

INT ([0-9]+)

%%

Monkey\ {INT}/:  {   this_monkey = monkeys + atoi(yytext+7);    }
"  "Starting\ items:\ ({INT},\ )*{INT} {   
    itemlist = init_itemlist(yytext + 18);
}

"  "Operation:\ new\ =\ old\ [\+\*]\ {INT}  {   
    oper = *(yytext+23) == '+' ? ADD : MUL;
    operand = atoi(yytext + 24);
}
"  "Operation:\ new\ =\ old\ \*\ old        {   oper = SQR; operand = 0;    }
"  "Test:\ divisible\ by\ {INT}             {   divisor = atoi(yytext+21);  }
"    "If\ true:\ throw\ to\ monkey\ {INT}   {   truthy = atoi(yytext+29);   }
"    "If\ false:\ throw\ to\ monkey\ {INT}  {   falsy = atoi(yytext+30);    }

^\n {   init_monkey(this_monkey, itemlist, oper, divisor, operand, truthy, falsy);  }
\n  {   }
:   {   }

<<EOF>> {   init_monkey(this_monkey, itemlist, oper, divisor, operand, truthy, falsy);  
            for(int i = 0; i < 10000; i++) round_monkeys(1);
            printf("%8lld\n",top_2());
            return 0;   }

%%

int compare(const void *a, const void *b)
{
    return ( *(long long*)a - *(long long*)b );
}

unsigned long long top_2(void)
{
    unsigned long long sortme[8];

    for(int i = 0; i < 8; i++) sortme[i] = monkeys[i].inspections;

    qsort(sortme,8,sizeof(unsigned long long),compare);

    return sortme[6] * sortme[7];

}