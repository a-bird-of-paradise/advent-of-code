%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
%define api.value.automove
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

%token <long long> NUMBER
%token ENDL SOIL FERTILISER WATER LIGHT HUMIDITY LOCATION TEMPERATURE COLON SEED

%type <aoc::triple> triple
%type <std::vector<aoc::triple>> triple_list
%type <std::vector<long long>> number_list

%nterm file seed_list seed_to_soil soil_to_fertiliser fertiliser_to_water water_to_light 
%nterm light_to_temperature temperature_to_humidity humidity_to_location
%start file

%%

file
    :
    seed_list 
    seed_to_soil 
    soil_to_fertiliser 
    fertiliser_to_water 
    water_to_light 
    light_to_temperature 
    temperature_to_humidity 
    humidity_to_location
    ;

seed_list
    :
    SEED COLON number_list ENDL ENDL
    {
        Context.seeds = $3;
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
        $$ = $1;
        $$.push_back($2);
    }
    ;

seed_to_soil
    :
    SEED SOIL COLON ENDL triple_list ENDL
    {
        Context.seed_to_soil = $5;
    }

soil_to_fertiliser
    :
    SOIL FERTILISER COLON ENDL triple_list ENDL
    {
        Context.soil_to_fertiliser = $5;
    }

fertiliser_to_water
    :
    FERTILISER WATER COLON ENDL triple_list ENDL
    {
        Context.fertiliser_to_water = $5;
    }

water_to_light
    :
    WATER LIGHT COLON ENDL triple_list ENDL
    {
        Context.water_to_light = $5;
    }

light_to_temperature
    :
    LIGHT TEMPERATURE COLON ENDL triple_list ENDL
    {
        Context.light_to_temperature = $5;
    }

temperature_to_humidity
    :
    TEMPERATURE HUMIDITY COLON ENDL triple_list ENDL
    {
        Context.temperature_to_humidity = $5;
    }

humidity_to_location
    :
    HUMIDITY LOCATION COLON ENDL triple_list
    {
        Context.humidity_to_location = $5;
    }

triple_list
    :
    triple
    {
        $$.push_back($1);
    }
    |   triple_list triple
    {
        $$ = $1;
        $$.push_back($2);
    }

triple
    :
    NUMBER NUMBER NUMBER ENDL
    {
        $$ = aoc::triple($1,$2,$3);
    }
    ;
    