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

%token  <int64_t>   INTEGER
%token  ENDL COMMA AT

%nterm file lines
%start file

%%

file
    :
    lines
    ;

lines
    :
    lines line
    |   line
    ;

line
    :
    INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER ENDL
    {
        Context.lines.push_back({$1,$2,$3,$4,$5,$6});
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
