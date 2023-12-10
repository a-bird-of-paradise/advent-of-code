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

%token <char> PIPE

%nterm file pipes
%start file

%%

file
    :
    pipes
    ;

pipes
    :
    PIPE
    {
        aoc::point point{@1.begin.column,@1.begin.line};
        Context.pipes[point]=$1;
    }
    | pipes PIPE
    {
        aoc::point point{@2.begin.column,@2.begin.line};
        Context.pipes[point]=$2;
    }
    ;