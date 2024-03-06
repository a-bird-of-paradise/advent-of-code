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

%token COLON ENDL CM HASH IN SPACE
%token <std::string> KEY
%token <std::string> HEX
%token <std::string> STRING
%token <int64_t> INT FOUR_INT NINE_INT
%nterm space_or_ENDL ENDL_or_YYEOF
%nterm <aoc::value_type> value 
%nterm <std::pair<std::string,aoc::value_type>> key_value_pair_inner key_value_pair
%nterm <aoc::passport_t> key_value_pairs passport
%start file

%%

file
    :
    passports
    ;

passports
    :
    passports passport
    {
        Context.passports.push_back(std::move($2));
    }
    |   passport
    {
        Context.passports.push_back(std::move($1));
    }
    ;

passport
    :
    key_value_pairs ENDL_or_YYEOF
    {
        $$ = std::move($1);
    }
    ;

key_value_pairs
    :
    key_value_pairs key_value_pair
    {
        $$ = std::move($1);
        $$[std::move($2.first)] = std::move($2.second);
    }
    | key_value_pair
    {
        $$[std::move($1.first)] = std::move($1.second);
    }
    ;

key_value_pair
    :
    key_value_pair_inner space_or_ENDL
    {
        $$ = std::move($1);
    }
    ;

key_value_pair_inner
    :
    KEY COLON value
    {
        $$.first = std::move($1);
        $$.second = std::move($3);
    }
    ;

value
    :
    INT
    {
        $$.t = aoc::data_type::integer;
        $$.int_val = $1;
    }
    |
    FOUR_INT
    {
        $$.t = aoc::data_type::four_int;
        $$.four_int_val = $1;
    }
    |
    NINE_INT
    {
        $$.t = aoc::data_type::nine_int;
        $$.nine_int_val = $1;
    }
    |   INT CM
    {
        $$.t = aoc::data_type::cm;
        $$.cm_val = $1;
    }
    |   INT IN
    {
        $$.t = aoc::data_type::in;
        $$.in_val = $1;
    }
    |   HASH HEX
    {
        $$.t = aoc::data_type::hex;
        $$.hex_val = std::move($2);
    }
    |   HEX
    {
        $$.t = aoc::data_type::string;
        $$.string_val = std::move($1);
    }
    |   STRING
    {
        $$.t = aoc::data_type::string;
        $$.string_val = std::move($1);
    }
    ;

space_or_ENDL
    :
    SPACE
    |   ENDL
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
