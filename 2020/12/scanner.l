%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
N   {   return aoc::parser::make_N(Context.loc);    }
E   {   return aoc::parser::make_E(Context.loc);    }
W   {   return aoc::parser::make_W(Context.loc);    }
S   {   return aoc::parser::make_S(Context.loc);    }
L   {   return aoc::parser::make_L(Context.loc);    }
R   {   return aoc::parser::make_R(Context.loc);    }
F   {   return aoc::parser::make_F(Context.loc);    }

[1-9][0-9]* {   return aoc::parser::make_INT(atoll(YYText()), Context.loc); }

\n      {   Context.loc.lines(); 
            //return aoc::parser::make_ENDL(Context.loc);     
        }
        
.       {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);    }
