%option 8bit fast align read noyywrap nomain header-file="lex.yy.h"

%{
#include "valves.h"
int yy_flow_rate = 0;
char yy_label[2];
%}

INT [-+]?[0-9]+
VALVE [A-Z]{2}

%x ADD_EDGES

%%

Valve   |
has     |
flow    |
rate    |
\       |
=       {   /* skip */ }

{VALVE} {   strncpy(yy_label,yytext,2); n_valves++;                         }

{INT}   {   yy_flow_rate = atoi(yytext); if(yy_flow_rate>0) used_valves++;  }

";" {   VALVE *this_valve = find_valve(yy_label); // finds or creates such valve
    this_valve->flow_rate = yy_flow_rate;
    BEGIN ADD_EDGES;    
}

<ADD_EDGES>{

tunnels?    |
leads?      |
to          |
valves?     |
\           |
,           { /* skip */  }

{VALVE} {   add_edge(find_valve(yy_label),find_valve(yytext));  }

\n      {   BEGIN INITIAL;  }

}

<<EOF>> { return 0; }
