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

%token ENDL

%token <int64_t> NUMBER
%type <std::vector<int64_t>> line_inner
%nterm file lines line
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
    line_inner ENDL
    {
        Context.histories.push_back($1);
    }
    ;

line_inner
    :
    NUMBER
    {
        $$.push_back($1);
    }
    |   line_inner NUMBER
    {
        $$ = $1;
        $$.push_back($2);
    }
