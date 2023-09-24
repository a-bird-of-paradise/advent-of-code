%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  Context.loc.step(); Context.loc.columns(yyleng); 
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
    if (!Context.tokens.empty()) return pop_token(Context);

[0-9A-Fa-f]     {   
    std::bitset<4> y = strtol(YYText(),nullptr,16);
    for(int i = 3; i >=0; i--) {
        if(y[i] == 0) { Context.tokens.push(0);}
        else { Context.tokens.push(1); }
    }
    return pop_token(Context);
}

\n              {   return aoc::parser::make_ENDL(Context.loc);     }
.               {   return aoc::parser::make_YYUNDEF(Context.loc);  }

<<EOF>>         {   return aoc::parser::make_YYEOF(Context.loc);    }
