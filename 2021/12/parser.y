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

%token ENDL HYPHEN 
%token <std::string> SMALL LARGE

%nterm pair pairs pair_inner opt_endl

%%
pairs:  pair | pairs pair;
opt_endl: %empty | ENDL;
pair: pair_inner opt_endl;

pair_inner
    :
    SMALL HYPHEN SMALL
    {
        Context.small_nodes.insert($1);
        Context.small_nodes.insert($3);
        Context.graph[$1].push_back($3);
        Context.graph[$3].push_back($1);
    }
    |   SMALL HYPHEN LARGE
    {
        Context.small_nodes.insert($1);
        Context.large_nodes.insert($3);
        Context.graph[$1].push_back($3);
        Context.graph[$3].push_back($1);
    }
    |   LARGE HYPHEN SMALL 
    {
        Context.large_nodes.insert($1);
        Context.small_nodes.insert($3);
        Context.graph[$1].push_back($3);
        Context.graph[$3].push_back($1);
    }
    |   LARGE HYPHEN LARGE
    {
        Context.large_nodes.insert($1);
        Context.large_nodes.insert($3);
        Context.graph[$1].push_back($3);
        Context.graph[$3].push_back($1);
    }
    ;

%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

