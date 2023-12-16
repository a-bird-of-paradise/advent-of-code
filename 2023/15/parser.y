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

%token ENDL EQUALS MINUS COMMA
%token <uint16_t> NUMBER
%token <std::string> CHARS

%nterm file item items comma_or_endl

%start file

%%

file
    :
    items
    {
        for(uint64_t box = 0; box < 256; ++box)
            for(uint64_t num = 0; num < Context.boxes[box].size(); ++num)
                Context.answer_2 += (box+1) * (num+1) * Context.boxes[box][num].power;
    }
    ;

items
    :
    item
    |   items item
    ;

item
    :
    CHARS EQUALS NUMBER comma_or_endl
    {
        std::string to_hash = $1 + '=' + std::to_string($3);
        Context.answer_1 += hash(to_hash);

        std::vector<std::vector<aoc::lens>>::iterator box;
        box = std::next(std::begin(Context.boxes), hash($1));

        std::vector<aoc::lens>::iterator x =
            std::find(
                std::begin(*box),
                std::end(*box),
                $1
            );
        
        if(x == std::end(*box)) {
            box->push_back({$1,$3});           
        } else {
            x->power = $3;    
        }
    }
    |   CHARS MINUS comma_or_endl
    {
        std::string to_hash = $1 + '-';
        Context.answer_1 += hash(to_hash);

        std::erase_if(Context.boxes[hash($1)], [this](lens x){return $1 == x.label;});
    }
    ;

comma_or_endl
    :
    COMMA
    |   ENDL
    ;