%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
contains    {                                                           }
[a-z]+      {   return aoc::parser::make_STRING(YYText(), Context.loc); }
"("         {   return aoc::parser::make_LPAREN(Context.loc);           }
")"         {   return aoc::parser::make_RPAREN(Context.loc);           }

,           {                                                           }
" "         {                                                           }

\n          {   Context.loc.lines();                                    }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);          }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);            }
