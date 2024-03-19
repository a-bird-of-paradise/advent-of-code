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

%token NOP ACC JMP
%token <int32_t>    INT
%token ENDL 

%type <aoc::instruction> instruction 
%nterm instructions

%start file

%%

file
    :
    instructions
    {
        aoc::instruction x;
        x.op = aoc::opcode::stop;
        x.arg = 0;
        Context.instructions.push_back(x);
    }
    ;

instructions
    :
    instructions instruction
    {
        Context.instructions.push_back($2);
    }
    |   instruction
    {
        Context.instructions.push_back($1);
    }
    ;

instruction
    :
    JMP INT ENDL
    {
        $$.op = aoc::opcode::jmp;
        $$.arg = $2;
    }
    |   ACC INT ENDL
    {
        $$.op = aoc::opcode::acc;
        $$.arg = $2;
    }
    |   NOP INT ENDL
    {
        $$.op = aoc::opcode::nop;
        $$.arg = $2;
    }

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
