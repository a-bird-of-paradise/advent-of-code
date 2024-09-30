%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
[0-9]+      {   return aoc::parser::make_INT(atoi(YYText()), Context.loc);  }
Player\ 1:  {                                                               }
Player\ 2:  {   return aoc::parser::make_MARK(Context.loc);                 }

\n          {   Context.loc.lines();                                    }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);          }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);            }
