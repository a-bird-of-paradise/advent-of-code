%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
=           {   return aoc::parser::make_EQUALS(Context.loc);       }
"["         {   return aoc::parser::make_LBRACKET(Context.loc);     }
"]"         {   return aoc::parser::make_RBRACKET(Context.loc);     }
mem         {   return aoc::parser::make_MEM(Context.loc);          }
mask        {   return aoc::parser::make_MASK(Context.loc);         }

[01X]{36}   {   return aoc::parser::make_MASK_VAL(YYText(),Context.loc);                    }

[0-9]+      {   return aoc::parser::make_INT(strtoull(YYText(),nullptr,10), Context.loc);   }

" "         {                                                   }

\n          {   Context.loc.lines();                            }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);    }
