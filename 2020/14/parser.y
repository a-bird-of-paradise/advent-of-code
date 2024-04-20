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
#include "funcs.hpp"
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token MASK MEM LBRACKET RBRACKET EQUALS
%token <uint64_t> INT
%token <std::string> MASK_VAL

%nterm statement mem_statement mask_statement statements

%start file

%%

file
    :
    statements
    {
        uint64_t answer_1 = 0, answer_2 = 0;
        for(const auto & pairs : Context.memory  ) answer_1 += pairs.second;
        for(const auto & pairs : Context.memory_2) answer_2 += pairs.second;
        std::cout << answer_1 << '\n' << answer_2 << '\n';
    }
    ;

statements
    :
    statement
    |   statements statement
    ;

statement
    :
    mem_statement
    |   mask_statement
    ;

mask_statement
    :
    MASK EQUALS MASK_VAL
    {
        Context.zero_mask = get_zero_mask($3);
        Context.ones_mask = get_ones_mask($3);
        Context.floats = get_floats($3);
    }
    ;

mem_statement
    :
    MEM LBRACKET INT RBRACKET EQUALS INT
    {
        // part 1
        uint64_t to_write = $6;
        to_write |= Context.ones_mask;
        to_write &= Context.zero_mask;
        Context.memory[$3] = to_write;

        // part 2
        to_write = $3; 
        to_write |= Context.ones_mask;
        std::vector<uint64_t> all_addresses = get_addresses(to_write,Context.floats);
        for(const auto& x : all_addresses) Context.memory_2[x] = $6;
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
