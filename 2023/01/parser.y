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

%token ENDL
%token <std::string> LINE
%nterm lines file line
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
    LINE ENDL
    {
        long first_1, last_1, first_2, last_2;

        std::vector<std::size_t> characters(9),digits(9);

        for(std::size_t i = 0; i < 9; i++)  
            digits[i] = $1.find('1'+i);

        first_1 = std::distance(
            std::begin(digits), 
            std::min_element(std::begin(digits), std::end(digits))) + 1;

        characters[0]  = $1.find("one");
        characters[1]  = $1.find("two");
        characters[2]  = $1.find("three");
        characters[3]  = $1.find("four");
        characters[4]  = $1.find("five");
        characters[5]  = $1.find("six");
        characters[6]  = $1.find("seven");
        characters[7]  = $1.find("eight");
        characters[8]  = $1.find("nine");

        if(*std::min_element(std::begin(digits),std::end(digits))
            < *std::min_element(std::begin(characters),std::end(characters)))
        {
            first_2 = first_1;
        } else {
            first_2 = std::distance(
                std::begin(characters),
                std::min_element(std::begin(characters),std::end(characters))
            ) + 1;
        }

        for(std::size_t i = 0; i < 9; i++)  
            digits[i] = $1.rfind('1'+i);

        for(auto&& i : digits)
            if(i == std::string::npos)
                i = 0;

        last_1 = std::distance(
            std::begin(digits), 
            std::max_element(std::begin(digits), std::end(digits))) + 1;

        characters[0]  = $1.rfind("one");
        characters[1]  = $1.rfind("two");
        characters[2]  = $1.rfind("three");
        characters[3]  = $1.rfind("four");
        characters[4]  = $1.rfind("five");
        characters[5]  = $1.rfind("six");
        characters[6]  = $1.rfind("seven");
        characters[7]  = $1.rfind("eight");
        characters[8]  = $1.rfind("nine");

        for(auto&& i : characters)
            if(i == std::string::npos)
                i = 0;

        if(*std::max_element(std::begin(digits),std::end(digits))
            > *std::max_element(std::begin(characters),std::end(characters)))
        {
            last_2 = last_1;
        } else {
            last_2 = std::distance(
                std::begin(characters),
                std::max_element(std::begin(characters),std::end(characters))
            ) + 1;
        }

        Context.answer_1 += 10*first_1 + last_1;
        Context.answer_2 += 10*first_2 + last_2;

        std::cout << first_1 << ' ' << last_1 << ' ' << first_2 << ' ' << last_2 << '\n';

    }
    ;
