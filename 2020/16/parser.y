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
#include "funcs.hpp"
}

%code {
#include "scanner.hpp"
#define yylex Scanner.lex
}

%token COLON COMMA HYPHEN OR ENDL NEARBY_TICKETS YOUR_TICKET
%token <int64_t> INT
%token <std::string> NAME

%nterm <std::vector<int64_t>> int_list int_list_inner
%nterm <std::vector<std::vector<int64_t>>> int_list_list
%nterm ENDL_or_YYEOF nearby_tickets own_ticket

%nterm <boost::icl::interval_set<int64_t>> field
%nterm <std::vector<boost::icl::interval_set<int64_t>>> field_list
%start file

%%

file
    :
    fields own_ticket nearby_tickets
    ;

fields
    :
    field_list ENDL_or_YYEOF
    {
        Context.ranges = $1;
    }
    ;

field_list
    :
    field_list field
    {
        $$ = $1;
        $$.push_back($2);
    }
    |   field
    {
        $$.push_back($1);
    }
    ;

field
    :
    NAME COLON INT HYPHEN INT OR INT HYPHEN INT ENDL
    {
        Context.names.push_back($1);
        boost::icl::discrete_interval<int64_t> left ($3,$5+1);
        boost::icl::discrete_interval<int64_t> right($7,$9+1);
        boost::icl::interval_set<int64_t> ival;
        ival.insert(left);
        ival.insert(right);
        $$ = ival;
    }
    ;

own_ticket
    :
    YOUR_TICKET COLON ENDL int_list ENDL_or_YYEOF
    {
        Context.ticket = $4;
    }

nearby_tickets
    :
    NEARBY_TICKETS COLON ENDL int_list_list ENDL_or_YYEOF
    {
        Context.nearby_tickets = $4;
    }
    ;

int_list_list
    :
    int_list
    {
        $$.push_back($1);
    }
    |   int_list_list int_list
    {
        $$ = $1;
        $$.push_back($2);
    }
    ;

int_list
    :
    int_list_inner ENDL
    {
        $$ = $1;
    }
    ;

int_list_inner
    :
    int_list_inner  COMMA   INT
    {
        $$ = $1;
        $$.push_back($3);
    }
    |   INT
    {
        $$.push_back($1);
    }
    ;

ENDL_or_YYEOF
    :
    ENDL
    |   YYEOF
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
