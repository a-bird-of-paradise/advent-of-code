%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
[0-9]+          {   return aoc::parser::make_NUMBER(atol(YYText()),Context.loc);    }

red             {   return aoc::parser::make_RED(Context.loc);      }
blue            {   return aoc::parser::make_BLUE(Context.loc);     }
green           {   return aoc::parser::make_GREEN(Context.loc);    }
Game            {   return aoc::parser::make_GAME(Context.loc);     }


":"             {   return aoc::parser::make_COLON(Context.loc);        }
";"             {   return aoc::parser::make_SEMICOLON(Context.loc);    }
","             {   return aoc::parser::make_COMMA(Context.loc);        }
" "             {                                                       }

\n              {   Context.loc.lines();
                    return aoc::parser::make_ENDL(Context.loc);     }

.               {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);    }
