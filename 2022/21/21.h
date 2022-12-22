#pragma once
#include <stdbool.h>
#include <stdarg.h>
// lexer declarations

extern int yylineno;
int yylex(void);

// symbol table

typedef struct symbol_t {
    char *name;
    long long value;
    bool evaluated; 
    struct ast_t *def;
    struct symbol_t *l;
    struct symbol_t *r;
}   SYMBOL;

#define N_HASH 9997

extern SYMBOL symtab[N_HASH];

SYMBOL *lookup(char *s);

// tree nodes

typedef struct ast_t {
    int nodetype;
    struct ast_t *l;
    struct ast_t *r;
} AST;

typedef struct numval_t {
    int nodetype;
    long long number;
} NUMNODE;

typedef struct symref_t {
    int nodetype;
    SYMBOL *s;
} SYMREF;

typedef struct symasgn_t {
    int nodetype;
    SYMBOL *s;
    AST *v;
} SYMASGN;

AST *newast(int nodetype, AST *l, AST *r);
AST *newref(SYMBOL *s);
AST *newasgn(SYMBOL *s, AST *v);
AST *newnum(long long x);

long long eval(AST *x);
void treefree(AST *x);

long long part2(void);

long long symbol_value (char *s);