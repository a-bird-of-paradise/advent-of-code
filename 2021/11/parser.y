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

%token ENDL
%token <int> NUMBER

%nterm file

%type < std::vector< std::vector<int> > > lines
%type <std::vector<int> > list line 
%start file

%%
file:   lines       {   Context.lines = $1; };

lines
    :
    line            {   $$.push_back($1);           }
    |   lines line  {   $$=$1; $$.push_back($2);    }
    ;

line:   list ENDL;

list
    :
    NUMBER          {   $$.push_back($1);           }
    |   list NUMBER {   $$=$1; $$.push_back($2);    }
    ;

%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

