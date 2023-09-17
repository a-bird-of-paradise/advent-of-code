%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(yyleng); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[a-zA-Z]        {   return aoc::parser::make_LETTER(*YYText(), Context.loc);            }
[a-zA-Z]{2}     {   return aoc::parser::make_PAIR(YYText(), Context.loc);               }
[a-zA-Z]+       {   return aoc::parser::make_TEMPLATE(YYText(), Context.loc);           }
" -> "          {   return aoc::parser::make_ARROW(Context.loc);                        }
\n              {   Context.loc.lines(); return aoc::parser::make_ENDL(Context.loc);    }
.               {   return aoc::parser::make_YYUNDEF(Context.loc);                      }
<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);                        }
