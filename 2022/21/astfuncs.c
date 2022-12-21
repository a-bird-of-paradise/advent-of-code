#include "21.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "21.tab.h"

SYMBOL symtab[N_HASH];

static unsigned symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned c;
    while((c=*sym++)) hash = 9*hash + c;
    return hash;
}

SYMBOL *lookup(char *sym)
{
    SYMBOL *sp = &symtab[symhash(sym) % N_HASH];

    int scount = N_HASH;

    while(--scount >= 0)
    {
        // found it! return
        if(sp->name && !strcmp(sp->name,sym)) return sp;

        // first lookup for hash! create and return
        if(!sp->name) {
            sp->name = strdup(sym);
            sp->value=0;
            sp->def = NULL;
            sp->evaluated = false;
            return sp;
        }

        // try next slot, looping around if necessary
        if(++sp >= symtab + N_HASH) sp = symtab;
    }

    yyerror("Symbol table full!\n");

    abort();
}

AST *newast(int nodetype, AST *l, AST *r)
{
    AST *a = malloc(sizeof(AST));
    if(!a) abort();
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

AST *newnum(long long num)
{
    NUMNODE *a = malloc(sizeof(NUMNODE));
    if(!a) abort();
    a->nodetype = 'K'; // Konstant
    a->number = num;
    return (AST *)a;
}

AST *newref(SYMBOL *s)
{
    SYMREF *a = malloc(sizeof(SYMREF));
    if(!a) abort();
    a->nodetype = 'N'; // Name
    a->s = s;
    return (AST *)a;
}

AST *newasgn(SYMBOL *s, AST *v)
{
    SYMASGN *a = malloc(sizeof(SYMASGN));
    if(!a) abort();
    a->nodetype=':';
    a->s = s;
    a->v = v;
    s->def=v;
    return (AST *)a;
}

void treefree(AST *a)
{
    // not now...
    return;
}

long long eval (AST *a)
{
    long long v = 0;
    if(!a) { yyerror("Null node\n"); return v;  }

    switch(a->nodetype) {
        case 'K':   v = ((NUMNODE *)a)->number; break;
        case 'N':   
        if(((SYMREF *)a)->s->evaluated) {
            v = ((SYMREF *)a)->s->value;
        } else {
            v = eval(((SYMREF *)a)->s->def);
            ((SYMREF *)a)->s->evaluated = true;
            ((SYMREF *)a)->s->value  = v;
        } 
        break;
        case ':':   ((SYMASGN *)a)->s->def = ((SYMASGN *)a)->v; break;
        case '+':   v = eval(a->l) + eval(a->r); break;
        case '-':   v = eval(a->l) - eval(a->r); break;
        case '*':   v = eval(a->l) * eval(a->r); break;
        case '/':   v = eval(a->l) / eval(a->r); break;
        default:    printf("Can't evaluate node [%c]\n",a->nodetype);
    }

    return v;
}

void reset_symbols(void)
{
    for(int i = 0; i < N_HASH; i++)
    {
        if(symtab[i].evaluated){
            symtab[i].evaluated = false;
            symtab[i].value = 0;
        }
    }
}

long long part2()
{
    SYMBOL *root = lookup("root");
    SYMBOL *humn = lookup("humn");
    AST *left = root->def->l;
    AST *right = root->def->r;

    printf("%lld %lld %lld\n",eval(left), eval(right), eval(right)-eval(left));

    long long *my_num = &(((NUMNODE *)(humn->def))->number), my_const = 3272260910000; 

    int i = 0;

    while(i < 10000)
    {
        reset_symbols();

        *my_num = my_const + i;

        eval(root->def);

        if(eval(right)-eval(left) == 0) return *my_num;
        i++;
    }
    return 0;
}