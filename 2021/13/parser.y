%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
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

%token ENDL COMMA EQUALS
%token <int> NUMBER
%token <char> LETTER

%nterm pair pairs pair_inner

%%

file:   pairs ENDL folds;
pairs:  pair | pairs pair;
pair:   pair_inner ENDL;
folds:  fold | folds fold;
fold:   fold_inner ENDL;

pair_inner: NUMBER COMMA NUMBER     { Context.points.emplace_back(std::make_pair($1,$3));   };
fold_inner: LETTER EQUALS NUMBER    { Context.folds.emplace_back(std::make_pair($1,$3));    }

%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

