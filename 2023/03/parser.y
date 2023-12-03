%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
%define api.value.automove
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

%token <char> SYMBOL
%token ENDL
%token <long> NUMBER

%type <aoc::symbol> symbol

%nterm file lines line line_inner
%start file

%%

file   
    :
    lines
    ;

lines
    :
    lines line
    |   line
    ;

line
    :
    line_inner ENDL
    ;

line_inner
    :
    NUMBER
    {
        Context.numbers.push_back(aoc::number($1, 
            @1.begin.line,
            @1.begin.column,
            @1.end.column-1)
        );       
    }
    | symbol
    {
        Context.symbols.push_back($1);
    }
    |   line_inner NUMBER
    {
        Context.numbers.push_back(aoc::number($2, 
            @2.begin.line,
            @2.begin.column,
            @2.end.column-1)
        );       
    }
    |   line_inner symbol
    {
        Context.symbols.push_back($2);
    }
    ;

symbol
    :
    SYMBOL
    {
        $$ = aoc::symbol($1,@1.begin.line,@1.begin.column);
    }
    ;