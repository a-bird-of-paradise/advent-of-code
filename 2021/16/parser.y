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

%token ZERO ONE ENDL TERMINATOR 
%nterm bit line

%type <int> version bit lead_length_bit 
%type <unsigned> length_inner
%type <long long> lead_data_segment lead_data_segments data_payload end_data_segment

%type <std::pair<int, long long> > length;

%type <aoc::packet> data_packet operator_packet packet
%type < std::vector<aoc::packet> > payload packets

%expect 1 // shifting here is exactly the right thing so permit it

%%

line
    :
    packet TERMINATOR trailing ENDL
    {
        std::cout << "Top value: " << $1.eval() << std::endl;
    }
    ;

bit
    :    
    ZERO 
    { 
        $$ = 0; 
    } 
    | ONE 
    { 
        $$ = 1; 
    }
    ;

packet
    : 
    operator_packet 
    {    
        if(Context.terminator_counter.top().first == 1) 
            Context.terminator_counter.top().second--;   
        $$ = $1;
    }
    | data_packet   
    {    
        if(Context.terminator_counter.top().first == 1) 
            Context.terminator_counter.top().second--;   
        $$ = $1;
    }
    ;


packets
    : 
    packet              {   $$.push_back($1);          }
    | packets packet    {   $$=$1; $$.push_back($2);   }
    ;

data_packet
    : 
    version ONE ZERO ZERO data_payload
    {
        $$.version = $1;
        $$.type_id = 4;
        $$.data_value = $5;
    }
    ;

operator_packet
    : 
    version     ZERO ZERO ZERO payload  
    {   
        $$.version = $1;
        $$.type_id = 0;
        $$.children = $5;
    }
    |   version ZERO ZERO ONE  payload  
    {   
        $$.version = $1;
        $$.type_id = 1;
        $$.children = $5;
    }
    |   version ZERO ONE  ZERO payload  
    {   
        $$.version = $1;
        $$.type_id = 2;
        $$.children = $5;
    }
    |   version ZERO ONE  ONE  payload  
    {   
        $$.version = $1;
        $$.type_id = 3;
        $$.children = $5;
    }
 // |   version ONE  ZERO ZERO payload
    |   version ONE  ZERO ONE  payload  
    {   
        $$.version = $1;
        $$.type_id = 5;
        $$.children = $5;
    }
    |   version ONE  ONE  ZERO payload  
    {   
        $$.version = $1;
        $$.type_id = 6;
        $$.children = $5;
    }
    |   version ONE  ONE  ONE  payload  
    {   
        $$.version = $1;
        $$.type_id = 7;
        $$.children = $5;
    }
    ;

version
    : 
    bit bit bit 
    { 
        $$ = 4*$1 + 2*$2 + 1*$3; 
        Context.version_sum += $$;
    }
    ;

payload
    : 
    length packets TERMINATOR {   $$ = $2;    }
    ;

length
    :
    lead_length_bit length_inner TERMINATOR 
    {   
        $$.first = $1; 
        $$.second = $2;  
        int shift = $$.first == 0 ? Context.bit_counter : 0;
        Context.terminator_counter.push(std::make_pair($$.first,$$.second + shift));
    }
    ;

lead_length_bit
    :
    ZERO    
    {   
        $$ = 0; 
        Context.terminator_counter.push(std::make_pair(0, Context.bit_counter+15));  
    }
    |   ONE 
    {   
        $$ = 1; 
        Context.terminator_counter.push(std::make_pair(0, Context.bit_counter+11));
    }
    ;

length_inner
    :
    bit                     {   $$ = $1;                        }
    |   length_inner bit    {   $$ = $1; $$ <<= 1; $$ += $2;    }
    ;

data_payload
    : 
    lead_data_segments end_data_segment
    {
        $$ = $1; $$ <<= 4; $$ += $2; 
    }
    ;

lead_data_segments
    : 
    %empty                                      {   $$ = 0;                         }   
    |   lead_data_segment                       {   $$ = $1;                        }
    |   lead_data_segments lead_data_segment    {   $$ = $1; $$ <<= 4; $$ += $2;    }
    ;

lead_data_segment
    : 
    ONE bit bit bit bit 
    {   
        $$ = 8*$2 + 4*$3 + 2*$4 + 1*$5; 
    }
    ;

end_data_segment
    : 
    ZERO bit bit bit bit 
    {   
        $$ = 8*$2 + 4*$3 + 2*$4 + 1*$5; 
    }
    ;

trailing
    : 
    %empty 
    | zeroes
    ;

zeroes
    : 
    ZERO 
    | zeroes ZERO
    ;
