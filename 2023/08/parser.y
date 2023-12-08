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
#include <algorithm>
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token EQUALS LPAREN RPAREN COMMA ENDL

%token <std::string> LR NODE

%nterm file line lines
%start file

%%

file
    :
    LR ENDL ENDL lines
    {
        Context.lr = std::move($1);
    }
    ;

lines
    :
    line
    | lines line
    ;

line
    :
    NODE EQUALS LPAREN NODE COMMA NODE RPAREN ENDL
    {
        Context.nodes[$1] = std::make_pair($4,$6);
    }
    ;
