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

%token ENDL AMPERSAND PERCENT ARROW BROADCASTER COMMA
%token <std::string> NAME

%type <std::vector<std::string>> namelist

%nterm file rule rules
%start file

%%

file
    :
    rules
    {

        std::map<std::string,std::vector<std::string>> conj;

        for(const auto & mod: Context.modules) {
            for(const auto & dest : mod.second.destinations) {
                if(Context.modules[dest].type == aoc::node_type::CONJUNCTION) {
                    conj[dest].push_back(mod.first);
                }
            }
        }

        for(const auto & c : conj)
            for(const auto & n : c.second)
                Context.modules[c.first].conjunction_memory[n]=false;
        
        for(auto&& mod : Context.modules)
            if(mod.second.type == aoc::node_type::BROADCASTER && mod.first != "broadcaster") 
                mod.second.type = aoc::node_type::SPECIAL;
        
        aoc::module the_module;
        the_module.type = aoc::node_type::BUTTON;
        the_module.destinations.push_back("broadcaster");
        the_module.output_high = false;
        Context.modules["BUTTON"] = std::move(the_module);    
        
    }
    ;

rules
    :
    rules rule
    |   rule
    ;

rule
    :
    BROADCASTER ARROW namelist ENDL
    {
        aoc::module the_module;
        the_module.type = aoc::node_type::BROADCASTER;
        the_module.destinations = std::move($3);
        the_module.output_high = false;
        Context.modules["broadcaster"] = std::move(the_module);
    }
    |   PERCENT NAME ARROW namelist ENDL
    {
        aoc::module the_module;
        the_module.type = aoc::node_type::FLIPFLOP;
        the_module.destinations = std::move($4);
        the_module.flip_flop_state = false;
        the_module.output_high = false;
        Context.modules[$2] = std::move(the_module);
    }
    |   AMPERSAND NAME ARROW namelist ENDL
    {
        aoc::module the_module;
        the_module.type = aoc::node_type::CONJUNCTION;
        the_module.destinations = std::move($4);
        the_module.output_high = false;
        Context.modules[$2] = std::move(the_module);
    }
    ;

namelist
    :
    namelist COMMA NAME
    {
        $$ = std::move($1);
        $$.push_back(std::move($3));
    }
    |   NAME
    {
        $$.push_back(std::move($1));
    }
    ;
