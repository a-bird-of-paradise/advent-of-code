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
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <std::string> ROW
%token ENDL

%nterm file algo field

%start file

%%

file
    :
    algo ENDL field
    ;

algo
    :
    ROW ENDL
    {
        std::vector<uint8_t> temp;
        for(std::size_t i = 0; i < 512; i++) {
            temp.push_back($1[i] == '.' ? 0 : 1);
        }
        Context.algo = std::move(temp);
    }
    ;

field
    :
    ROW ENDL
    {
        std::vector<uint8_t> temp;
        for(std::size_t i = 0; i < $1.size(); i++) {
            temp.push_back($1[i] == '.' ? 0 : 1);
        }
        Context.field.push_back(std::move(temp));
    }
    |
    field ROW ENDL
    {
        std::vector<uint8_t> temp;
        for(std::size_t i = 0; i < $2.size(); i++) {
            temp.push_back($2[i] == '.' ? 0 : 1);
        }
        Context.field.push_back(std::move(temp));
    }
    ;
    

