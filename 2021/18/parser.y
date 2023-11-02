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
#include "Tree.hpp"
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <int> NUMBER
%token L_ANGLE R_ANGLE COMMA ENDL

%nterm <std::unique_ptr<aoc::Node>> item 
%nterm <std::unique_ptr<aoc::BranchNode>> line pair
%nterm <std::vector<std::unique_ptr<aoc::BranchNode>>> lines
%nterm file

%start file

%%

file
    :   
    lines   
    {
        Context.SnailNumbers = $1;
    }
    ;

lines
    :
    line            
    {   
        $$.push_back($1);
    }
    |   lines line  
    {   
        $$ = $1;
        $$.push_back($2);
    }
    ;

line
    :
    pair ENDL
    {
        $$ = $1;
    }
    ;

pair
    :
    L_ANGLE item COMMA item R_ANGLE 
    {
        $$ = std::make_unique<aoc::BranchNode>($2,$4);
    }
    ;

item
    :
    NUMBER      
    {   
        $$ = std::make_unique<aoc::LeafNode>($1);              
    }
    |   pair    
    {   
        $$ = $1;
    }
    ;

