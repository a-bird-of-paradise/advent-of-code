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
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token <int> NUMBER
%token COMMA ENDL SCANNER_ROW

%nterm file endl_or_yyeof

%type <aoc::point> position
%type <std::vector<aoc::point>> positions scanner
%type <std::vector<std::vector<aoc::point>>> scanners 

%start file

%%

file
    :
    scanners
    {
        Context.scanners = $1;
    }
    ;

scanners
    :
    scanners scanner
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   scanner
    {
        $$.push_back($1);   
    }
    ;

scanner
    :
    SCANNER_ROW ENDL positions endl_or_yyeof
    {
        $$ = $3;
    }
    ;

positions
    :
    positions position
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   position
    {
        $$.push_back($1);
    }
    ;

position
    :
    NUMBER COMMA NUMBER COMMA NUMBER ENDL
    {
        $$ = aoc::point($1,$3,$5);
    }
    ;

endl_or_yyeof
    :
    ENDL
    |   YYEOF
    ;
