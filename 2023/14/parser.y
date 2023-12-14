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

%token <std::string> CHARS
%token ENDL

%nterm file lines

%type <std::string> line
%start file

%%

file
    :
    lines
    ;

lines
    :
    lines line
    {
        Context.lines.push_back($2);
    }
    | line
    {
        Context.lines.push_back($1);
    }
    ;


line
    :
    CHARS ENDL
    {
        $$ = $1;
    }
    ;
