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

%token <int64_t>    NUMBER
%token COMMA ENDL TILDE
%type <aoc::brick> line
%nterm file lines
%start file

%%

file
    :
    lines
    ;

lines
    :
    line 
    {
        Context.bricks.push_back($1);
    }
    |   lines line
    {
        Context.bricks.push_back($2);
    }
    ;

line
    :
    NUMBER COMMA NUMBER COMMA NUMBER TILDE NUMBER COMMA NUMBER COMMA NUMBER ENDL
    {
        $$ = aoc::brick($1,$3,$5,$7,$9,$11);
    }
    ;