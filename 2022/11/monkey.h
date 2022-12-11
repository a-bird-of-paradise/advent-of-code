#pragma once

typedef enum operation { MUL, ADD, SQR } OP;

typedef struct item {
    long long worry_level;
    struct item *next;
} ITEM;

typedef struct monkey {
    ITEM *first_item;
    int worry_divisor, true_monkey, false_monkey;
    long long inspections;
    OP op;
    int operand;
} MONKEY;

void init_monkey(MONKEY *x, 
    ITEM *items,
    OP op,
    int divisor,
    int operand,
    int true_m,
    int false_m);

ITEM *init_itemlist(char *csl);
void print_monkeys(void);

void round_monkeys(int div);