%require "3.8"
%language "C++"
%defines "parser.hpp"
%output "parser.cpp"

%locations

%define api.namespace {aoc}
%define api.parser.class {parser}
%define api.token.constructor
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

%token ENDL
%token <int> LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE LANGLE RANGLE

%nterm lines line chunks any
%start lines

%destructor {  /*
    error state in the token destructor means:
    -n: we are reducing and have n terminals left to pop off the stack (not an error!)
    0: we have just entered error recovery, so set to 1 to mark we are in error recovery
    1: we are in phase 1 of error recovery (popping stack).
    2: we are in phase 2 of error recovery (discarding new tokens) - set by lexer if in phase 1 */
    if(aoc::error_state <= 0 ) aoc::error_state++;                  
    if(aoc::error_state == 1 && $$ != 0) { aoc::answer_2 *= 5; aoc::answer_2 += $$; }
} LPAREN  LBRACE  LBRACKET LANGLE RPAREN RBRACE RBRACKET RANGLE

%%

lines:  line | lines line;

line
    : 
    chunks ENDL                         {    }                         
    |   chunks error ENDL               
    {   
        Context.line_totals.push_back(answer_2); 
        aoc::error_state = 0;
        aoc::answer_2 = 0; 
    }
    |   chunks error RANGLE any ENDL    {   Context.answer_1 += 25137;  aoc::error_state = -1; aoc::answer_2 = 0; $3; }
    |   chunks error RPAREN any ENDL    {   Context.answer_1 += 3;      aoc::error_state = -1; aoc::answer_2 = 0; $3; }
    |   chunks error RBRACE any ENDL    {   Context.answer_1 += 1197;   aoc::error_state = -1; aoc::answer_2 = 0; $3; }
    |   chunks error RBRACKET any ENDL  {   Context.answer_1 += 57;     aoc::error_state = -1; aoc::answer_2 = 0; $3; }
    ;

chunks
    :
    %empty
    |   chunks LPAREN chunks RPAREN     {   aoc::error_state = -2; $2; $4; }
    |   chunks LBRACKET chunks RBRACKET {   aoc::error_state = -2; $2; $4; }
    |   chunks LBRACE chunks RBRACE     {   aoc::error_state = -2; $2; $4; }
    |   chunks LANGLE chunks RANGLE     {   aoc::error_state = -2; $2; $4; }
    ;

any 
    :   /* special error only non terminal - so leave error_state alone */
    %empty
    |   any LPAREN      {   $2; }
    |   any RPAREN      {   $2; }
    |   any LANGLE      {   $2; }
    |   any RANGLE      {   $2; }
    |   any LBRACE      {   $2; }
    |   any RBRACE      {   $2; }
    |   any LBRACKET    {   $2; }
    |   any RBRACKET    {   $2; }
    ;

%%

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

