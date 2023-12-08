%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit
%x NODES
%%
[LR]+       {   BEGIN NODES;
                return aoc::parser::make_LR(YYText(),Context.loc);  }    
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);        }  

<NODES>{
[A-Z0-9]{3} {   return aoc::parser::make_NODE(YYText(),Context.loc);    }

"="     {   return aoc::parser::make_EQUALS(Context.loc);   }
"("     {   return aoc::parser::make_LPAREN(Context.loc);   }
")"     {   return aoc::parser::make_RPAREN(Context.loc);   }
","     {   return aoc::parser::make_COMMA(Context.loc);    }
" "     {   }

\n      {   Context.loc.lines();    return aoc::parser::make_ENDL(Context.loc);     }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);                          }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);                            }
}
