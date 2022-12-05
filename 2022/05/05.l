%option noyywrap main 8bit fast align

%{
#include <stdbool.h>

struct crate {
    char payload;
    struct crate *next;
};

struct stack {
    struct crate *top;
};

struct stack *stacks = 0;
struct stack *thisstack = 0;
struct stack *multistacks = 0;
struct stack *thismstack = 0;

int nstacks = 0, tomove = 0, source = 0, destination = 0;

void freestacks(void), printstacks(void), stack(void), move(void), init(void);

%}

%x STACKING MOVING

%%

^.*\n           {  init();  BEGIN STACKING; }

<STACKING>{ 
\[.\]\ ?        {   stack();                                        }
\ \ \ \ ?       {   thisstack++; thismstack++;                      }
\n              {   thisstack = stacks; thismstack = multistacks;   }
\ 1.*\n\n       {   BEGIN MOVING;                                   }
}

<MOVING>{
move\ [0-9]+    {   tomove = atoi(yytext + 5);      }
\ from\ [0-9]+  {   source = atoi(yytext + 6);      }
\ to\ [0-9]+    {   destination = atoi(yytext + 4); }
\n              {   move();                         }
}

<<EOF>> {   printstacks();  freestacks();   return 0;   }

%%

void init(void)
{
    nstacks = yyleng / 4; 
    stacks = malloc(nstacks * sizeof(struct stack)); 
    multistacks = malloc(nstacks * sizeof(struct stack));
    for(int i = 0; i < nstacks; i++) {
        thisstack = stacks + i;
        thisstack->top = 0;
        thismstack = multistacks + i;
        thismstack->top = 0;
    }
    thisstack = stacks;
    thismstack = multistacks;
    yyless(0);
}

void freestacks(void)
{
    struct crate *current, *next;
    
    for(int i = 0; i < nstacks; ++i)
    {
        thisstack = stacks + i;
        current = thisstack->top;
        while(current)
        {
            next = current -> next;
            free(current);
            current = next;
        }

        thismstack = multistacks + i;
        current = thismstack->top;
        while(current)
        {
            next = current -> next;
            free(current);
            current = next;
        }
    }
    thisstack = 0;
    thismstack = 0;
    free(stacks);
    free(multistacks);
}

void printstacks(void)
{
    struct crate* current;
    for(int i = 0; i < nstacks; i++)
    {
        thisstack = stacks + i;
        current = thisstack->top;
        printf("%c",current->payload);
    }
    printf("\n");
    for(int i = 0; i < nstacks; i++)
    {
        thismstack = multistacks + i;
        current = thismstack->top;
        printf("%c",current->payload);
    }
    printf("\n");
}

void stack(void)
{
    struct crate* thiscrate = malloc(sizeof(struct crate));
    struct crate* currentcrate = 0;
    thiscrate->payload = yytext[1];
    thiscrate->next = 0;
    currentcrate = thisstack->top;
    if(!currentcrate) { // top of stack
        thisstack->top = thiscrate;
    }
    else {
        while(currentcrate->next) currentcrate = currentcrate->next;
        currentcrate->next = thiscrate;
    }
    thisstack++;

    thiscrate = malloc(sizeof(struct crate));
    currentcrate = 0;
    thiscrate->payload = yytext[1];
    thiscrate->next = 0;
    currentcrate = thismstack->top;
    if(!currentcrate) { // top of stack
        thismstack->top = thiscrate;
    }
    else {
        while(currentcrate->next) currentcrate = currentcrate->next;
        currentcrate->next = thiscrate;
    }
    thismstack++;           
}

void move(void)
{

    int tomove_save = tomove;

    struct crate *currentcrate = 0;
    struct stack *source_stack, *destination_stack;
    source_stack = stacks + source - 1;
    destination_stack = stacks + destination - 1;
    while(tomove)
    {
        currentcrate = source_stack->top;
        source_stack->top = currentcrate->next;
        currentcrate->next = destination_stack->top;
        destination_stack->top = currentcrate;
        tomove--;
    }

    tomove = tomove_save-1;

    source_stack = multistacks + source - 1;
    destination_stack = multistacks + destination - 1;

    struct crate *firstmove = 0, *lastmove = 0;

    firstmove = source_stack-> top;
    lastmove = firstmove;

    while(tomove--) {
        lastmove = lastmove -> next;
    }

    source_stack->top = lastmove->next;
    lastmove->next = destination_stack->top;
    destination_stack->top = firstmove;
}