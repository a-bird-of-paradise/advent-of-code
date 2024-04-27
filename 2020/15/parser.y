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
#include "funcs.hpp"
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token MASK MEM LBRACKET RBRACKET EQUALS
%token <uint64_t> INT
%token <std::string> MASK_VAL

%nterm statement mem_statement mask_statement statements

%start file

%%

file
    :
    statements
    ;

statements
    :
    statement
    |   statements statement
    ;

statement
    :
    mem_statement
    |   mask_statement
    ;

mask_statement
    :
    MASK EQUALS INT
    ;

mem_statement
    :
    MEM LBRACKET INT RBRACKET EQUALS INT
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
