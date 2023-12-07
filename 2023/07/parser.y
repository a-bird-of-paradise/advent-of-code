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
#include <algorithm>
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <int16_t> NUMBER
%token ENDL
%token <std::string> HAND

%nterm file line lines
%start file

%%

file
    :
    lines
    ;

lines
    :
    line
    | lines line
    ;

line
    :
    HAND NUMBER ENDL
    {
        Context.lines.emplace_back($1,$2);
    }
    ;
