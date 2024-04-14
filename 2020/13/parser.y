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
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token COMMA X
%token <int64_t> INT

%nterm line

%start file

%%

file
    :
    INT line            {   Context.minutes = $1;       }
    ;

line
    :
    line COMMA X        {   Context.line.push_back(0);  }
    |   line COMMA INT  {   Context.line.push_back($3); }
    |   X               {   Context.line.push_back(0);  }
    |   INT             {   Context.line.push_back($1); }
    ;


%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
