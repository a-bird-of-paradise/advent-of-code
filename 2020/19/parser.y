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

%token COLON ENDL PIPE QUOTATION
%token <std::string> STRING
%token <int> INT

%nterm string_line rule_line string_lines rule_lines
%nterm terminal_rule nonterminal_rule
%type <aoc::ints> ints

%start file

%%

file
    :
    rule_lines ENDL string_lines
    ;

rule_lines
    :
    rule_lines rule_line
    |   rule_line
    ;


rule_line
    :
    terminal_rule
    |   nonterminal_rule
    ;

terminal_rule
    :
    INT COLON QUOTATION STRING QUOTATION ENDL
    {
        Context.rules[$1] = { aoc::rule_type::terminal, $4[0] };
    }
    ;

nonterminal_rule
    :
    INT COLON ints ENDL
    {
        aoc::intss to_push;
        to_push.push_back(std::move($3));
        Context.rules[$1] = { aoc::rule_type::nonterminal, '!', std::move(to_push) };
    }
    |   INT COLON ints PIPE ints ENDL
    {
        aoc::intss to_push;
        to_push.push_back(std::move($3));
        to_push.push_back(std::move($5));
        Context.rules[$1] = { aoc::rule_type::nonterminal, '!', std::move(to_push) };
    }
    ;

ints
    :
    ints INT
    {
        $$ = std::move($1);
        $$.push_back($2);
    }
    |   INT
    {
        $$.push_back($1);
    }
    ;

string_lines
    :
    string_lines string_line
    |   string_line
    ;

string_line
    :
    STRING ENDL
    {
        Context.strings.push_back(std::move($1));
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
