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

%initial-action
{
    Context.space.resize(101);
    for(auto & i : Context.space) {
        i.resize(101);

        for(auto & j : i) {
            j.resize(101);
            for(auto & k : j) {
                k = 0;
            }
        }
    }
};

%token <long> INT 
%token ENDL ON OFF EQUALS DOTS X Y Z COMMA

%nterm file line lines switch xdef ydef zdef

%start file

%%

file
    :
    lines
    {
        long answer = 0;
        for(auto const & i : Context.space) {
            for(auto const & j : i) {
                for(auto const & k : j) {
                    answer += k;
                }
            }
        }

        std::cout << answer << std::endl;
    }
    ;

lines
    :
    line
    | lines line
    ;

line
    :
    switch xdef COMMA ydef COMMA zdef ENDL
    {

        aoc::cuboid new_cube(Context.start_x,Context.end_x,
            Context.start_y, Context.end_y,
            Context.start_z, Context.end_z);

        Context.cuboids.push_back(std::make_pair(new_cube,Context.is_on));

        if(Context.start_x >= -50 && Context.end_x <= 50
            && Context.start_y >= -50 && Context.end_y <= 50
            && Context.start_z >= -50 && Context.end_z <= 50){

            for(std::size_t i = 50 + Context.start_x; i < 51 + Context.end_x; i++) {
                for(std::size_t j = 50 + Context.start_y; j < 51 + Context.end_y; j++) {
                    for(std::size_t k = 50 + Context.start_z; k < 51 + Context.end_z; k++) {
                        Context.space[i][j][k] = Context.is_on ? 1 : 0;
                    }
                }
            }
        }
    }
    ;

switch
    :
    ON      
    {
        Context.is_on = true;
    }
    |   OFF
    {
        Context.is_on = false;
    }
    ;

xdef
    :
    X EQUALS INT DOTS INT
    {
        Context.start_x = $3;
        Context.end_x = $5;
    }
    ;

ydef
    :
    Y EQUALS INT DOTS INT
    {
        Context.start_y = $3;
        Context.end_y = $5;
    }
    ;

zdef
    :
    Z EQUALS INT DOTS INT
    {
        Context.start_z = $3;
        Context.end_z = $5;
    }
    ;
