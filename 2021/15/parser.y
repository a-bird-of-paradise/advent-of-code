%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.error detailed
%define parse.trace

%parse-param {aoc::scanner& Scanner}{aoc::context& Context}
%lex-param {aoc::context& Context}

%code requires {
namespace aoc { class scanner; }
#include "context.hpp"
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token ENDL 
%token <int> NUMBER

%type <std::vector<int> > line_inner

%nterm lines line

%%

lines:  line | lines line;
line
    :   
    line_inner ENDL         {   Context.board.push_back($1);    }
    ;

line_inner
    :
    NUMBER                  {   $$.push_back($1);               }
    |   line_inner NUMBER   {   $$=$1; $$.push_back($2);        }
    ;
