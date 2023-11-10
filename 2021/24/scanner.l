%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[w-z]           {   return aoc::parser::make_REGISTER(*YYText(),Context.loc);       }
[+-]?[0-9]+     {   return aoc::parser::make_INTEGER(atol(YYText()), Context.loc);  }

inp             {   return aoc::parser::make_INP(Context.loc);                      }
add             {   return aoc::parser::make_ADD(Context.loc);                      }
mul             {   return aoc::parser::make_MUL(Context.loc);                      }
mod             {   return aoc::parser::make_MOD(Context.loc);                      }
div             {   return aoc::parser::make_DIV(Context.loc);                      }
eql             {   return aoc::parser::make_EQL(Context.loc);                      }

" "             {                                                                   }
\n              {   Context.loc.lines();
                    return aoc::parser::make_ENDL(Context.loc);                     }
<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);                    }
