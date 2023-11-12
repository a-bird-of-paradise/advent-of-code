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
#include <iomanip>
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token ENDL

%token <char> CELL
%type <std::vector<char>> line cells
%type <std::vector<std::vector<char>>> lines
%nterm file
%start file

%%

file
    :
    lines
    {
        Context.lines = $1;
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
    cells ENDL
    {
        $$ = $1;
    }
    ;

cells
    :
    CELL
    {
        $$.push_back($1);
    }
    |   cells CELL
    {
        $$ = $1;
        $$.push_back($2);
    }
