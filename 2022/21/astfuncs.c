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
        case 'K': break;

        default: break;
    }
    free(a);
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

long long part2()
{
    SYMBOL *root = lookup("root");
    SYMBOL *humn = lookup("humn");

    SYMBOL *this = humn, *next, *other;

    char path[400][5] = {0};
    int path_length = 0;

    while(this != root)
    {
        strcpy(path[path_length],this->name);
        path_length++;

        for(int i = 0; i < N_HASH; i++)
        {
            next = &symtab[i];
            if(next->name) {
                if(next->l == this) {
                    this = next;
                    break;
                } else if (next->r == this) {
                    this = next;
                    break;
                }
            } 
        }
    }
        
    strcpy(path[path_length],this->name);

    long long new_value, prev_value, operand;

    this = lookup("root");

    if(!strcmp(this->l->name, path[path_length-1])) {
        prev_value = this->r->value; next = this->l; other = this->r;
    } else {
        prev_value = this->l->value; next = this->r; other = this->l;
    }

    printf("%s l:%s:%16lld r:%s:%16lld op:%c next:%s %16lld\n",
        this->name, this->l->name, this->l->value,
        this->r->name,this->r->value,this->def->nodetype,next->name, prev_value);

    for (int i = path_length - 1; i > 0; i--) 
    {

        this = lookup(path[i]);

        if(!strcmp(this->l->name, path[i-1])) {
            next = this->l; other = this->r;
        } else {
            next = this->r; other = this->l;
        }

        // this node's value was produced from l and r via op 
        // the other value is constant, so what value do we need to set 
        // new value to be, to recover prev value? 

        switch(this->def->nodetype){
            case '+': new_value = prev_value - other->value; break;
            case '-': new_value = prev_value + other->value; break;
            case '*': new_value = prev_value / other->value; break;
            case '/': new_value = prev_value * other->value; break;
            default: printf("%c\n",this->def->nodetype);
        }

        printf("%s l:%s:%16lld r:%s:%16lld op:%c next:%s %16lld\n",
            this->name, this->l->name, this->l->value,
            this->r->name,this->r->value,this->def->nodetype,next->name, new_value);

        prev_value = new_value;
    }

    return new_value;
}