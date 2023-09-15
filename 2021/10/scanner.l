%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION  \
    Context.loc.step(); \
    Context.loc.columns(yyleng); \
    if(aoc::error_state >= 1) aoc::error_state = 2;
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%
"("     {   return aoc::parser::make_LPAREN(1,Context.loc);     }
")"     {   return aoc::parser::make_RPAREN(0,Context.loc);     }
"["     {   return aoc::parser::make_LBRACKET(2,Context.loc);   }
"]"     {   return aoc::parser::make_RBRACKET(0,Context.loc);   }
"{"     {   return aoc::parser::make_LBRACE(3,Context.loc);     }
"}"     {   return aoc::parser::make_RBRACE(0,Context.loc);     }
"<"     {   return aoc::parser::make_LANGLE(4,Context.loc);     }
">"     {   return aoc::parser::make_RANGLE(0,Context.loc);     }

\n      {   Context.loc.lines(); return aoc::parser::make_ENDL(Context.loc);    }
.       {   return aoc::parser::make_YYUNDEF(Context.loc);                      }
<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);                        }
