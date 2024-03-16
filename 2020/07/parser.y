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

%token <std::string> STRING
%token <int32_t>    INT
%token ENDL 

%nterm bag bags 

%nterm <std::vector<aoc::bag_count_t>> bag_list bag_list_inner

%start file

%%

file
    :
    bags
    ;

bags
    :
    bags bag
    |   bag
    ;

bag
    :
    STRING STRING bag_list ENDL
    {
        for(const auto& bag : $3)
        {
            Context.parents[bag.first].push_back(std::make_pair($1,$2));
        }
        
        Context.bags[std::make_pair($1,$2)] = std::move($3);
    }
    ;

bag_list
    :
    %empty              {               }
    |   bag_list_inner  {   $$ = $1;    }
    ;

bag_list_inner
    :
    INT STRING STRING
    {
        aoc::bag_count_t x;
        x.second = $1;
        x.first.first = $2;
        x.first.second = $3;
        $$.push_back(x);
    }
    |   bag_list_inner INT STRING STRING
    {
        $$ = $1;
        aoc::bag_count_t x;
        x.second = $2;
        x.first.first = $3;
        x.first.second = $4;
        $$.push_back(x);
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
