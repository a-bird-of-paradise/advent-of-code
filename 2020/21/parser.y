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

%token <std::string> STRING
%token LPAREN RPAREN

%nterm file lines line

%type <std::set<std::string>> strings

%start file

%%

file:       lines;
lines:      lines line | line;
line
    :       
    strings LPAREN strings RPAREN
    {
        Context.lines.push_back(std::make_pair($1,$3));
    }
    ;

strings
    :
    strings STRING
    {
        $$ = $1;
        $$.insert($2);
    }
    | STRING
    {
        $$.insert($1);
    }
    ;
%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
