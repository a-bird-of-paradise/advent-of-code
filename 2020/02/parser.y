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

%token <int>   NUMBER
%token <char> CHAR
%token <std::string> STRING
%nterm file lines line
%start file

%%

file
    :
    lines
    {
        std::cout << Context.answer_1 << '\n';
        std::cout << Context.answer_2 << '\n';
    }
    ;

lines
    :
    line
    |   lines line
    ;

line
    :
    NUMBER NUMBER CHAR STRING
    {
        // part 1
        int64_t n = 0;
        for(auto i = 0; i < $4.size(); ++i) if($4[i] == $3) n++;
        if(n >= $1 && n <= $2)  Context.answer_1++;
        // part 2
        n = 0;
        if($4[$1-1] == $3) n++;
        if($4[$2-1] == $3) n++;
        if(n == 1) Context.answer_2++;
    }
    ;
%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
