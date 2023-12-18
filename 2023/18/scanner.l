%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
[0-9]+/" "  {   return aoc::parser::make_NUMBER(atoi(YYText()),Context.loc);    }
[RDLU]/" "  {   return aoc::parser::make_CHAR(*YYText(), Context.loc);          }
[a-f0-9]{6} {   return aoc::parser::make_COLOUR(YYText(), Context.loc);         }

"(" |
")" |
"#" |
" " {   }

\n      {   Context.loc.lines(); 
            return aoc::parser::make_ENDL(Context.loc);     }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);    }
