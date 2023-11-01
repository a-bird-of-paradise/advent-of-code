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

%token <char> LETTER
%token <int> NUMBER
%token EQUALS ENDL

%nterm line 
%%

line
    :
    LETTER EQUALS NUMBER NUMBER LETTER EQUALS NUMBER NUMBER ENDL
    {
        Context.xmin = $3;
        Context.xmax = $4;
        Context.ymin = $7;
        Context.ymax = $8;
    }
    ;
