%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[ABCD]          {   return aoc::parser::make_LETTER(*YYText(),Context.loc);         }
\n              {   Context.loc.lines();                                            }
[^ABCD\n]+      {                                                                   }
<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);                    }
