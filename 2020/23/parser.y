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

%token <int> INT

%type <std::vector<int>> ints

%start file

%%

file
    :
    ints 
    {
        Context.input = $1;
    }
    ;

ints
    :
    INT
    {
        $$.push_back($1);
    }
    |   ints INT
    {
        $$ = $1;
        $$.push_back($2);
    }
%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
