#include "21.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "21.tab.h"

SYMBOL symtab[N_HASH] = {0};

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

    // if v is arithmetic, note the left and right syms in the expression 

    if(v){
        switch(v->nodetype){
            case '+':
            case '-':
            case '*':
            case '/':
                s->l = ((SYMREF *)(v->l))->s;
                s->r = ((SYMREF *)(v->r))->s;
            default: break;
        }
    }

    return (AST *)a;
}

void treefree(AST *a)
{
    switch(a->nodetype) {
        case '+': 
        case '-': 
        case '*': 
        case '/': treefree(a->r); treefree(a->l); break;
        
        case 'L': if(a->r) treefree(a->r); if (a->l) treefree(a->l); break;

        case ':': treefree( ((SYMASGN *)a)->v); break;

        case 'N': 
        case 'K': 
        default: break;
    }
    free(a);
    return;
}


long long symbol_value (char *s)
{
    SYMBOL *sym = lookup(s);
    if(!sym->evaluated) {
        sym->value = eval(sym->def);
        sym->evaluated = true;
    }
    return sym->value;
}

long long eval (AST *a)
{
    long long v = 0;
    if(!a) { yyerror("Null node\n"); return v;  }

    switch(a->nodetype) {
        case 'K':   v = ((NUMNODE *)a)->number; break;
        case 'N':   v = symbol_value( ((SYMREF *)a)->s->name); break; 
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

    bool found = false; 

    long long lower = INT64_MIN/256, upper = INT64_MAX/256, current;
    long long lower_val, upper_val, current_val;
    long long lower_diff, upper_diff, current_diff;
    long long answer, orig;

    orig = humn->value;

    while(!found) {

        current = (upper + lower) / 2;

        reset_symbols();
        humn->evaluated = true; humn->value = lower; lower_val = symbol_value("root");
        lower_diff = root->l->value - root->r->value;
        reset_symbols();
        humn->evaluated = true; humn->value = upper; upper_val = symbol_value("root");
        upper_diff = root->l->value - root->r->value;
        reset_symbols();
        humn->evaluated = true; humn->value = current; current_val = symbol_value("root");
        current_diff = root->l->value - root->r->value;

        if((current_diff < 0 && lower_diff < 0)
            || (current_diff > 0 && lower_diff > 0)) {
            lower = current;
        } else {
            upper = current;
        }

        printf("%20lld%20lld%20lld%20lld\n",current,current_diff,lower_diff,upper_diff);

        if(current_diff == 0) found=true;
    }

    answer =  humn->value;
    current = answer;

    printf("\nCandidate answers have 0 as the second column:\n");

    for(long long i = current - 3; i <= current + 3; i++)
    {
        reset_symbols();
        humn->evaluated = true; 
        humn->value = i; 
        current_val = symbol_value("root");
        current_diff = root->l->value - root->r->value;
        printf("%20lld%20lld\n",i,current_diff);
        if(current_diff == 0 && i < answer) answer = i;
    }

    reset_symbols(); 
    humn->value = orig;
    current_val = symbol_value("root");

    return answer;
}