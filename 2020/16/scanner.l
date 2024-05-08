%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
:               {   return aoc::parser::make_COLON(Context.loc);            }
"-"             {   return aoc::parser::make_HYPHEN(Context.loc);           }
,               {   return aoc::parser::make_COMMA(Context.loc);            }
or              {   return aoc::parser::make_OR(Context.loc);               }
nearby\ tickets {   return aoc::parser::make_NEARBY_TICKETS(Context.loc);   }
your\ ticket    {   return aoc::parser::make_YOUR_TICKET(Context.loc);      }

[a-z]+          |
[a-z]+\ [a-z]+  {   return aoc::parser::make_NAME(YYText(),Context.loc);    }

[0-9]+      {   return aoc::parser::make_INT(strtoll(YYText(),nullptr,10), Context.loc);    }

" "         {                                                   }

\n          {   Context.loc.lines();                            
                return aoc::parser::make_ENDL(Context.loc);     }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);    }
