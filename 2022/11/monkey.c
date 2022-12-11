#include "monkey.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.h"

extern MONKEY monkeys[8];
extern int modulo;

static int inited = 0;

void init_monkey(MONKEY *x, 
    ITEM *items,
    OP op,
    int divisor,
    int operand,
    int true_m,
    int false_m)
{
    x->first_item = items;
    x->worry_divisor = divisor;
    x->op = op;
    x->operand = operand;
    x->true_monkey = true_m;
    x->false_monkey = false_m;
    x->inspections = 0;

    if(divisor != 0) modulo *= divisor;
}

ITEM *init_itemlist(char *csl)
{
    int csl_length = 0, this_int;
    char *csl_it = csl;
    while(*csl_it++) csl_length++;

    ITEM *head = NULL, *last_item = NULL, *this_item = NULL;
    
    csl_it = strtok(csl,",");
    while(csl_it)
    {
        this_int = atoi(csl_it);
        this_item = malloc(sizeof(ITEM));
        this_item->worry_level = this_int;
        this_item->next = NULL;

        if(!head)
        {
            head = this_item;
        }
        else
        {
            last_item->next = this_item;
        }
        last_item = this_item;

        csl_it = strtok(NULL,",");
    }

    return head;

}

void print_monkeys(void)
{
    for(int i = 0; i < 8;  i++)
    {
        MONKEY *x = &monkeys[i];
        printf("[%p][%p]%4d%4d%4d%4d%4d%8lld\n",
            x, x->first_item,x->worry_divisor,x->op,
            x->operand,x->true_monkey,x->false_monkey,
            x->inspections);

        ITEM *this_item = x->first_item;

        while(this_item)
        {
            printf("[%p][%p]%4d\n", this_item, this_item->next, this_item->worry_level);
            this_item = this_item->next;
        }
    }
}

void monkey_throw(ITEM *the_item, int dest_monkey)
{
    ITEM *last_item = monkeys[dest_monkey].first_item;
    if(last_item)
    {
        while(last_item->next) last_item = last_item->next;
        last_item->next = the_item;
    } else {
        monkeys[dest_monkey].first_item = the_item;     
    }
    the_item->next=NULL;
}

void monkey_turn(int i,int div)
{
    ITEM *this_item = monkeys[i].first_item, *next_item;
    monkeys[i].first_item = NULL;
    while(this_item)
    {
        monkeys[i].inspections++;
        next_item = this_item->next;

        if(monkeys[i].op == ADD)
        {
            this_item->worry_level = 
                (this_item->worry_level + monkeys[i].operand) % modulo;
        }
        else if(monkeys[i].op == MUL)
        {
            this_item->worry_level =
                (this_item->worry_level * monkeys[i].operand) % modulo;
        }
        else if(monkeys[i].op == SQR)
        {
            this_item->worry_level = 
                (this_item->worry_level * this_item->worry_level) % modulo;
        }

        this_item->worry_level /= div;

        if(this_item->worry_level % monkeys[i].worry_divisor == 0)
        {
            monkey_throw(this_item,monkeys[i].true_monkey);
        } else {
            monkey_throw(this_item,monkeys[i].false_monkey);
        }

        this_item = next_item;

    }
}

void round_monkeys(int div)
{
    for(int i = 0; i < 8; i++)
        monkey_turn(i,div);
}