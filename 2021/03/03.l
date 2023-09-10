%{
#include "scanner.internal.hpp"
#include <algorithm>
#undef  YY_DECL
#define YY_DECL int aoc::scanner::lex()
%}

%option c++ noyywrap outfile="03.lex.cpp" batch yyclass="aoc::scanner" prefix="aoc" 8bit

%%

[01]+   {   rows.push_back(YYText());   column = yyleng;    return 1; }

\n      {   row++;  return 1; }

<<EOF>> {   return 0;   }
