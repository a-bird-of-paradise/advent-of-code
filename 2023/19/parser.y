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

%token ENDL COLON LT GT COMMA LBRACE RBRACE EQUALS A R
%token <int64_t> NUMBER
%token <char> CHAR
%token <std::string> NAME

%nterm file workflows parts workflow part

%type <aoc::rule> rule
%type <std::vector<aoc::rule>> rules

%start file

%%

file
    :
    workflows ENDL parts
    ;

workflows
    :
    workflows workflow
    |   workflow
    ;

parts
    :
    parts part
    |   part
    ;

workflow
    :
    NAME LBRACE rules RBRACE ENDL
    {
        Context.workflows[$1] = $3;
    }
    ;

rules
    :
    rules COMMA rule
    {
        $$ = $1;
        $$.push_back($3);
    }
    |   rule
    {
        $$.push_back($1);
    }
    ;

rule
    :
    CHAR LT NUMBER COLON NAME
    {   
        $$.type = 'M';  
        $$.compare = '<'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.name = $5;
    }
    |   CHAR GT NUMBER COLON NAME
    {   
        $$.type = 'M';  
        $$.compare = '>'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.name = $5;
    }
    |   CHAR LT NUMBER COLON A 
    {   
        $$.type = 'C';  
        $$.compare = '<'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.is_accept = true;
    }
    |   CHAR GT NUMBER COLON A
    {   
        $$.type = 'C';  
        $$.compare = '>'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.is_accept = true;
    }
    |   CHAR LT NUMBER COLON R      
    {   
        $$.type = 'C';  
        $$.compare = '<'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.is_accept = false;
    }
    |   CHAR GT NUMBER COLON R      
    {   
        $$.type = 'C';  
        $$.compare = '>'; 
        $$.letter = $1; 
        $$.num = $3;  
        $$.is_accept = false;
    }
    |   R                           
    {   
        $$.type = 'R';                  
    }
    |   A                           
    {   
        $$.type = 'A';                  
    }
    |   NAME                        
    {   
        $$.type = 'N'; 
        $$.name = $1;    
    }
    ;

part
    :
    LBRACE CHAR EQUALS NUMBER COMMA
        CHAR EQUALS NUMBER COMMA
        CHAR EQUALS NUMBER COMMA
        CHAR EQUALS NUMBER RBRACE ENDL
    {
        Context.parts.push_back(aoc::part($4,$8,$12,$16));
    }
    ;
    