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

%token N E W S L R F
%token <int64_t> INT

%nterm lines line

%start file

%%

file:   lines   
    {   
        std::cout << llabs(Context.x)  + llabs(Context.y)  << '\n';  
        std::cout << llabs(Context.sx) + llabs(Context.sy) << '\n';   
    }
    ;

lines:  lines line  
    |   line;

line
    :
    N INT       {   Context.y += $2;    Context.wy += $2;   }
    |   E INT   {   Context.x += $2;    Context.wx += $2;   }
    |   S INT   {   Context.y -= $2;    Context.wy -= $2;   }
    |   W INT   {   Context.x -= $2;    Context.wx -= $2;   }
    |   L INT   
    {   
        Context.angle += $2;    
        Context.angle = mod(Context.angle,360LL); 

        int64_t dx  = Context.wx - Context.sx;
        int64_t dy  = Context.wy - Context.sy;

        switch($2)
        {
            case    90:
                Context.wx = Context.sx - dy;
                Context.wy = Context.sy + dx;
                break;
            case    180:
                Context.wx = Context.sx - dx;
                Context.wy = Context.sy - dy;
                break;
            case    270:
                Context.wx = Context.sx + dy;
                Context.wy = Context.sy - dx;
                break;
            default:
                assert(false);
        }  
    }
    |   R INT   
    {   
        Context.angle -= $2;    
        Context.angle = mod(Context.angle,360LL);  

        int64_t dx  = Context.wx - Context.sx;
        int64_t dy  = Context.wy - Context.sy;

        switch($2)
        {
            case    270:
                Context.wx = Context.sx - dy;
                Context.wy = Context.sy + dx;
                break;
            case    180:
                Context.wx = Context.sx - dx;
                Context.wy = Context.sy - dy;
                break;
            case    90:
                Context.wx = Context.sx + dy;
                Context.wy = Context.sy - dx;
                break;
            default:
                assert(false);
        }
    }
    |   F INT   {
    
        int64_t dx  = Context.wx - Context.sx;
        int64_t dy  = Context.wy - Context.sy;

        Context.sx += $2 * dx;
        Context.sy += $2 * dy; 
        Context.wx = Context.sx + dx;
        Context.wy = Context.sy + dy; 

        switch(Context.angle)
        {
            case    0:
                // east
                Context.x += $2;
                break;
            case    90: 
                //  north
                Context.y += $2;
                break;
            case    180:
                //  west
                Context.x -= $2;
                break;
            case    270:
                //  south
                Context.y -= $2;
                break;
            default:
                assert(false);
        }
    }
    ;

%%

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
