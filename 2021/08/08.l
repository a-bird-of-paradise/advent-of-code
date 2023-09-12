%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION Context.loc.step(); Context.loc.columns(yyleng);
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[a-g]+  {   unsigned answer = 0;
            for(std::size_t i = 0; i < YYLeng(); i++)
                answer |= ( (unsigned)1 <<( YYText()[i] - 'a'));
            return aoc::parser::make_STRING(answer,Context.loc);
        }
\n      {   Context.loc.lines(); 
            return aoc::parser::make_ENDL(Context.loc);                     
        }
[ ]     {                                                       }
\|      {   return aoc::parser::make_SEPARATOR(Context.loc);    }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);      }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);        }
