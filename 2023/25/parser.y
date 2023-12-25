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

%token <std::string>   NAME
%token ENDL COLON
%type <std::vector<std::string>> namelist namelist_inner
%nterm file lines
%start file

%%

file
    :
    lines
    {
        for(auto&& g : Context.graph)
        {
            std::ranges::sort(g.second);
            auto ret = std::ranges::unique(g.second);
            g.second.erase(ret.begin(),ret.end());
        }
    }
    ;

lines
    :
    lines line
    |   line
    ;

line:
    NAME COLON namelist
    {
        for(const auto& n : $3)
        {
            Context.graph[$1].push_back(n);
            Context.graph[n].push_back($1);
        }
    }

    ;

namelist:
    namelist_inner ENDL
    {
        $$ = std::move($1);
    }
    ;

namelist_inner
    :
    NAME
    {
        $$.push_back(std::move($1));
    }
    |   namelist_inner NAME
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
