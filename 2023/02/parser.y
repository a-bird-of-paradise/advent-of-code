%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
//%define api.value.automove
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

%token ENDL GAME RED BLUE GREEN COMMA COLON SEMICOLON
%token <int> NUMBER

%nterm file

%type <aoc::colour> colour
%type <aoc::turn_t> turn_inner
%type <std::vector<aoc::turn_t>> turn
%type <aoc::turnlist_t> turnlist
%type <std::pair<int,aoc::turnlist_t>> line line_inner
%type <std::vector<std::pair<int,aoc::turnlist_t>>> lines

%start file

%%

file   
    :
    lines
    {
        Context.file = $1;
    }
    ;

lines
    :
    line
    {
        $$.push_back($1);
    }
    |   lines line
    {
        $$ = $1;
        $$.push_back($2);
    }
    ;

line
    :
    line_inner ENDL
    {
        $$ = $1;
    }
    ;

line_inner
    :
    GAME NUMBER COLON turnlist
    {
        $$ = std::make_pair($2,$4);
    }
    ;

turnlist
    :
    turn
    {
        $$.push_back($1);
    }
    |   turnlist SEMICOLON turn
    {
        $$ = $1;
        $$.push_back($3);
    }
    ;

turn
    :
    turn_inner
    {
        $$.push_back($1);
    }
    |   turn COMMA turn_inner
    {
        $$ = $1;
        $$.push_back($3);
    }
    ;

turn_inner
    :
    NUMBER colour
    {
        $$ = std::make_pair($2,$1);
    }
    ;

colour
    :
    RED         {   $$ = aoc::colour::RED;      }
    |   BLUE    {   $$ = aoc::colour::BLUE;     }
    |   GREEN   {   $$ = aoc::colour::GREEN;    }
    ;