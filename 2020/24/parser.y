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

%token E SE SW W NW NE ENDL
%nterm line lines file
%type <aoc::move> move moves
%start file

%%

file
    :   
    lines
    {
        std::cout << Context.flipped.size() << '\n';
    }
    ;
    
lines:  lines line | line;

line
    :  
    moves ENDL  
    {     
        if(Context.flipped.contains($1)) {
            Context.flipped.erase($1);
        } else {
            Context.flipped.insert($1);
        }
    }
    ;

moves
    :  
    moves move  {   $$ = $1 + $2;   }
    | move      {   $$ = $1;        }
    ;

move
    :   
    E       {   $$ = aoc::move( 1, 0);    }
    | SE    {   $$ = aoc::move( 0,-1);    }
    | SW    {   $$ = aoc::move(-1,-1);    }
    | W     {   $$ = aoc::move(-1, 0);    }
    | NW    {   $$ = aoc::move( 0, 1);    }
    | NE    {   $$ = aoc::move( 1, 1);    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
