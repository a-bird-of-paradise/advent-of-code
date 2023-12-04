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

%token PIPE COLON CARD ENDL
%token <long> NUMBER

%type <std::vector<long>> number_list

%nterm file lines line
%start file

%%

file   
    :
    lines
    {
        for(std::size_t i = 0; i < Context.winners.size(); i++)
        {
            // store original
            Context.win_pile[i+1] += 1;

            long k = Context.win_pile[i+1];

            while(k>0)
            {
                // add succeeding winners per copy 

                for(std::size_t j = 0; j < Context.winners[i]; j++) {
                    Context.win_pile[i+1 + j+1]++;
                }
                k--;
            }
        }

        for(const auto& pairs:Context.win_pile) Context.answer_2 += pairs.second;
    }
    ;

lines
    :
    lines line
    |   line
    ;

line
    :
    CARD NUMBER COLON number_list PIPE number_list ENDL
    {
        std::sort(std::begin($4),std::end($4));
        std::sort(std::begin($6),std::end($6));

        std::vector<long> intersection;
        std::set_intersection(
            std::begin($4),
            std::end($4),
            std::begin($6),
            std::end($6),
            std::back_inserter(intersection)
        );

        if(intersection.size() >= 1) {
            Context.answer_1 += std::pow(2,intersection.size()-1);
        }

        Context.winners.push_back(intersection.size());

    }
    ;

number_list
    :
    NUMBER
    {
        $$.push_back($1);
    }
    |   number_list NUMBER
    {
        $$ = std::move($1);
        $$.push_back($2);
    }
    ;