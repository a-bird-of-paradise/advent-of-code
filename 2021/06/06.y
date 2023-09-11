%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
%define api.token.prefix {TOK_}
%define api.value.type variant
%define parse.assert
%define parse.error detailed
%define parse.lac full
%define parse.trace

%parse-param {aoc::scanner& Scanner}{aoc::context& Context}
%lex-param {aoc::context& Context}

%code requires
{
namespace aoc { class scanner; }
#include "context.hpp"
}

%code
{
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token ENDL
%token COMMA
%token <long> NUMBER

%nterm file opt_endl
%nterm <std::vector<long> > list list_inner

%%

file:
    list    opt_endl   {   Context.list = $1; }
    ;

list: list_inner ENDL;

list_inner:
    NUMBER                          {   $$.push_back($1);               }
    |   list_inner COMMA NUMBER     {   $$ = $1;    $$.push_back($3);   }
    ;

opt_endl:   %empty  |   ENDL;

%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

