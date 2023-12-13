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

%token <char> CHAR 
%token ENDL
%nterm file notes
%type <std::vector<char>> chars line
%type <aoc::note> note
%start file

%%

file
    :
    notes
    ;

notes
    :
    note ENDL
    {
        Context.notes.push_back($1);
    }
    |   notes note ENDL
    {
        Context.notes.push_back($2);
    }
    |   notes note YYEOF
    {
        Context.notes.push_back($2);
    }
    ;

note
    :
    note line
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   line
    {
        $$.push_back($1);
    }
    ;

line
    :
    chars ENDL
    {
        $$ = $1;
    }
    ;

chars
    :
    chars CHAR
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   CHAR 
    {
        $$.push_back($1);
    }
    ;