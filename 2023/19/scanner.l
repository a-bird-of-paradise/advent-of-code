%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
[0-9]+      {   return aoc::parser::make_NUMBER(atoi(YYText()),Context.loc);    }
[xmas]      {   return aoc::parser::make_CHAR(*YYText(), Context.loc);          }
[a-z]{2,3}  {   return aoc::parser::make_NAME(YYText(), Context.loc);           }

:   {   return aoc::parser::make_COLON(Context.loc);    }
"<" {   return aoc::parser::make_LT(Context.loc);       }
">" {   return aoc::parser::make_GT(Context.loc);       }
,   {   return aoc::parser::make_COMMA(Context.loc);    }
"{" {   return aoc::parser::make_LBRACE(Context.loc);   }
"}" {   return aoc::parser::make_RBRACE(Context.loc);   }
=   {   return aoc::parser::make_EQUALS(Context.loc);   }
A   {   return aoc::parser::make_A(Context.loc);        }
R   {   return aoc::parser::make_R(Context.loc);        }

\n      {   Context.loc.lines(); 
            return aoc::parser::make_ENDL(Context.loc);     }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);    }
