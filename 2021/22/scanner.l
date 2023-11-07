%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

on              {   return aoc::parser::make_ON(Context.loc);       }
off             {   return aoc::parser::make_OFF(Context.loc);      }
x               {   return aoc::parser::make_X(Context.loc);        }
y               {   return aoc::parser::make_Y(Context.loc);        }
z               {   return aoc::parser::make_Z(Context.loc);        }
=               {   return aoc::parser::make_EQUALS(Context.loc);   }
".."            {   return aoc::parser::make_DOTS(Context.loc);     }
,               {   return aoc::parser::make_COMMA(Context.loc);    }

[+-]?[0-9]+     {   return aoc::parser::make_INT(atol(YYText()),Context.loc);       }
\n              {   Context.loc.lines();    
                    return aoc::parser::make_ENDL(Context.loc);                     }

" "             {                                                                   }
.               {   return aoc::parser::make_YYUNDEF(Context.loc);                  }
<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);                    }
