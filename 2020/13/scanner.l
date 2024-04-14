%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
X|x         {   return aoc::parser::make_X(Context.loc);        }
,           {   return aoc::parser::make_COMMA(Context.loc);    }

[1-9][0-9]* {   return aoc::parser::make_INT(atoll(YYText()), Context.loc); }

\n      {   Context.loc.lines();                            }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);    }
