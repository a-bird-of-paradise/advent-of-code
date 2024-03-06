%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(YYLeng()); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
byr         |
iyr         |        
eyr         |        
hgt         |        
hcl         |        
ecl         |        
pid         |        
cid         {   return aoc::parser::make_KEY(YYText(),Context.loc);     } 

":"         {   return aoc::parser::make_COLON(Context.loc);    }
"#"         {   return aoc::parser::make_HASH(Context.loc);     }
cm          {   return aoc::parser::make_CM(Context.loc);       }
in          {   return aoc::parser::make_IN(Context.loc);       }

[0-9]+/in   {   return aoc::parser::make_INT(atoll(YYText()),Context.loc);  }
[0-9]+/cm   {   return aoc::parser::make_INT(atoll(YYText()),Context.loc);  }

[0-9]{4}    {   return aoc::parser::make_FOUR_INT(atoll(YYText()),Context.loc);     }
[0-9]{9}    {   return aoc::parser::make_NINE_INT(atoll(YYText()),Context.loc);     }

[a-f0-9]{6} {   return aoc::parser::make_HEX(YYText(),Context.loc);         }
[0-9]+      {   return aoc::parser::make_INT(atoll(YYText()),Context.loc);  }

[a-z]+      {   return aoc::parser::make_STRING(YYText(),Context.loc);      }
" "         {   return aoc::parser::make_SPACE(Context.loc);    }

\n          {   Context.loc.lines(); 
                return aoc::parser::make_ENDL(Context.loc);     }
.           {   return aoc::parser::make_YYUNDEF(Context.loc);  }
<<EOF>>     {   return aoc::parser::make_YYEOF(Context.loc);    }
