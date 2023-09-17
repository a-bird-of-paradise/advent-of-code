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

%token ENDL ARROW
%token <std::string> TEMPLATE PAIR
%token <char> LETTER

%type <std::string> template;

%%

file:       template ENDL rules {   Context.Template = $1;  };
template:   TEMPLATE ENDL       {   $$ = $1;                };
rules:      rule | rules rule;
rule:       rule_inner ENDL;
rule_inner: PAIR ARROW LETTER   {   Context.rules.push_back(std::make_tuple($1[0],$1[1],$3));   };

