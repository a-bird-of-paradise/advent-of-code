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
#include <iomanip>
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <char> REGISTER
%token <long long> INTEGER
%token MUL ADD EQL DIV MOD INP ENDL

%nterm instruction 

%type <long long> operand

%%

instructions
    :
    instruction
    {
        std::cout << "         w: " << Context.registers['w'] << ", "
            << "x: " << Context.registers['x'] << ", "
            << "y: " << Context.registers['y'] << ", "
            << "z: " << Context.registers['z'] << ", " 
            << Context.ticker << std::endl;
        Context.ticker++;
    }
    |   instructions instruction
    {
        std::cout << "         w: " << Context.registers['w'] << ", "
            << "x: " << Context.registers['x'] << ", "
            << "y: " << Context.registers['y'] << ", "
            << "z: " << Context.registers['z'] << ", " 
            << Context.ticker << std::endl;
        Context.ticker++;
    }
    ;

instruction
    :
    MUL REGISTER operand ENDL
    {
        std::cout << "mul " << $2 << ' ';
        Context.registers[$2] *= $3;
    }
    |   ADD REGISTER operand ENDL
    {
        std::cout << "add " << $2 << ' ';
        Context.registers[$2] += $3;
        if(Context.ticker == 5)   Context.A.push_back($3);
        if(Context.ticker == 15)   Context.B.push_back($3);
    }
    |   EQL REGISTER operand ENDL
    {
        std::cout << "eql " << $2 << ' ';
        Context.registers[$2] = Context.registers[$2] == $3 ? 1 : 0;
    }
    |   DIV REGISTER operand ENDL
    {
        std::cout << "div " << $2 << ' ';
        Context.registers[$2] /= $3;
        if(Context.ticker == 4)   Context.d.push_back($3);
    }
    |   MOD REGISTER operand ENDL
    {
        std::cout << "mod " << $2 << ' ';
        Context.registers[$2] %= $3;
    }
    |   INP REGISTER ENDL
    {
        std::cout << "\n     inp " << $2 << " ";
        Context.registers[$2] = Context.input.top();
        Context.input.pop();
        Context.ticker = 0;
    }
    ;

operand
    :
    REGISTER
    {
        std::cout << std::setw(4) << $1 << ' ';
        $$ = Context.registers[$1];
    }
    |   INTEGER
    {
        std::cout << std::setw(4) << $1 << ' ';
        $$ = $1;
    }
    ;
