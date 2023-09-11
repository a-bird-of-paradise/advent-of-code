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
%token ARROW
%token <long> NUMBER

%nterm file
%nterm <aoc::point_t> point
%nterm <aoc::line_t> line
%nterm <std::vector<aoc::line_t> > lines

%%

file:
    lines   {   Context.lines = $1; }
    ;

lines:
    line            {   $$.push_back($1);           }       
    |   lines line  {   $$ = $1; $$.push_back($2);  }
    ;

line:
    point ARROW point ENDL  {   $$.first = $1; $$.second = $3;  }
    ;

point:  
    NUMBER COMMA NUMBER { $$.first = $1; $$.second = $3;    }
    ;
%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

