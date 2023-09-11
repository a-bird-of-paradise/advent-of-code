%{
#include "scanner.internal.hpp"
#undef  YY_DECL
#define YY_DECL aoc::parser::symbol_type aoc::scanner::lex(context& Context)
#define YY_USER_ACTION Context.loc.step(); Context.loc.columns(yyleng);
%}

%option c++ noyywrap outfile="scanner.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[0-9]+  {   return aoc::parser::make_NUMBER(atol(YYText()), Context.loc);   }
,       {   return aoc::parser::make_COMMA(Context.loc);                    }
\n      {   return aoc::parser::make_ENDL(Context.loc);                     }
.       {   std::cout << "Unexpected character " << *YYText() << std::endl; }

<<EOF>> {   return aoc::parser::make_YYEOF(Context.loc);                    }
