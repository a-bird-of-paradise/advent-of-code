%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
e           {   return aoc::parser::make_E(Context.loc);        }
se          {   return aoc::parser::make_SE(Context.loc);       }
sw          {   return aoc::parser::make_SW(Context.loc);       }
w           {   return aoc::parser::make_W(Context.loc);        }
nw          {   return aoc::parser::make_NW(Context.loc);       }
ne          {   return aoc::parser::make_NE(Context.loc);       }

\n          {   Context.loc.lines();                                        
                return aoc::parser::make_ENDL(Context.loc);     }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);    }
