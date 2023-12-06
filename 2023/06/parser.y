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

%token <uint64_t> NUMBER
%token ENDL

%type <std::vector<uint64_t>> list
%nterm file
%start file

%%

file
    :
    list ENDL list ENDL
    {
        Context.time = $1;
        Context.distance = $3;
    }
    ;

list
    :
    list NUMBER
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   NUMBER
    {
        $$.push_back($1);
    }
    ;