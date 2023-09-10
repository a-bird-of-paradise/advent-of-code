%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
%define api.token.prefix {TOK_}
%define api.value.type variant
%define parse.assert
%define parse.error detailed
%define parse.lac full
%define parse.trace

%parse-param {aoc::scanner& Scanner}{aoc::context& Context}
%lex-param {aoc::context& Context}

%code requires
{
namespace aoc { class scanner; }
#include "context.hpp"
}

%code
{
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token ENDL
%token COMMA
%token <long> NUMBER

%nterm <std::vector<long> > row list row_inner list_inner
%nterm <aoc::board_t> board
%nterm <std::vector< aoc::board_t> > boards
%nterm file opt_endl

%%

file: 
    list ENDL boards opt_endl   {   Context.random_numbers = $1; Context.boards = $3; }
    ;

list: 
    list_inner ENDL   {   $$ = $1;    }

list_inner: 
    NUMBER                      {   $$.push_back($1);           }
    |   list_inner COMMA NUMBER {   $$ = $1; $$.push_back($3);  }
    ;

boards: 
    board                   {   $$.push_back($1);           }
    |   boards ENDL board   {   $$=$1;  $$.push_back($3);   }
    ;

board:   
    row             {   $$.push_back($1);           }
    |   board row   {   $$=$1;  $$.push_back($2);   }    
    ;

row:   
    row_inner ENDL   {   $$ = $1;    }
    ;

row_inner:  
    NUMBER                  {   $$.push_back($1);           }
    |   row_inner NUMBER    {   $$ = $1; $$.push_back($2);  }
    ;

opt_endl:
    ENDL
    |   %empty
    ;
%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

