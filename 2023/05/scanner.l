%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
[0-9]+          {   return aoc::parser::make_NUMBER(atoll(YYText()),Context.loc);   }

seeds?          {   return aoc::parser::make_SEED(Context.loc);         }
soil            {   return aoc::parser::make_SOIL(Context.loc);         }
fertilizer      {   return aoc::parser::make_FERTILISER(Context.loc);   }
humidity        {   return aoc::parser::make_HUMIDITY(Context.loc);     }
temperature     {   return aoc::parser::make_TEMPERATURE(Context.loc);  }
water           {   return aoc::parser::make_WATER(Context.loc);        }
location        {   return aoc::parser::make_LOCATION(Context.loc);     }
light           {   return aoc::parser::make_LIGHT(Context.loc);        }

:               {   return aoc::parser::make_COLON(Context.loc);        }

" "+    |
"-to-"  |
map     {   }

\n      {   Context.loc.lines();
            return aoc::parser::make_ENDL(Context.loc);     }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);  }

<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);    }
