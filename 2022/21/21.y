%{
#include <stdio.h>
#include <stdlib.h>
#include "21.h"
%}

%define parse.error detailed

%code requires { #include "21.h" }
%code provides { void yyerror(const char *s, ...); }

%union  {
    AST *a;
    long long ll;
    SYMBOL *s;
}

%token <ll> NUMBER;
%token <s> NAME;
%token EOL;

%right ':'
%left '+' '-'
%left '*' '/'

%type <a> stmt exp list
%type <ll> file
%start file

%%

file:   list            {   
    printf("%lld\n", symbol_value("root"));
    printf("%lld\n",part2());
    treefree($1);
};

list:   stmt            { $$ = newast('L',$1,NULL); }
    |   list stmt       { $1->r = $2;  $$ = newast('L',$1,NULL); };

stmt: exp EOL           { $$ = $1;  };

exp:    exp '+' exp     { $$ = newast('+',$1,$3);   }
    |   exp '-' exp     { $$ = newast('-',$1,$3);   }
    |   exp '*' exp     { $$ = newast('*',$1,$3);   }
    |   exp '/' exp     { $$ = newast('/',$1,$3);   }
    |   NAME            { $$ = newref($1);          }
    |   NUMBER          { $$ = newnum($1);          }
    |   NAME ':' exp    { $$ = newasgn($1,$3);      };

%%

void yyerror(const char *s, ...)
{
    va_list ap;
    va_start(ap,s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr,s,ap);
    fprintf(stderr,"\n");
}