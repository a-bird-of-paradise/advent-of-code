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
#include <algorithm>
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <std::string> STRING
%token <int> INT

%type <aoc::tile> tile_inner

%nterm file tiles tile

%start file

%%

file:       tiles;
tiles:      tiles tile | tile;

tile
    :       
    INT tile_inner
    {
        Context.tiles[$1] = $2;
    }
    ;

tile_inner
    : 
    tile_inner STRING 
    {
        $$ = $1;
        $$.push_back($2);
    }
    |    STRING
    {
        $$.push_back($1);
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
