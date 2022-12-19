%option 8bit noyywrap header-file="lex.yy.h"

%{
#include "structs.h"
int yy_blueprint, yy_ore_cost, yy_clay_cost; 
int yy_obs_ore_cost, yy_obs_clay_cost; 
int yy_geode_ore_cost, yy_geode_obs_cost;

%}

INT [-+]?[0-9]+

%%

Blueprint\ {INT}                {   yy_blueprint = atoi(yytext+10); }
Each\ ore\ robot\ costs\ {INT}  {   yy_ore_cost = atoi(yytext+21);  }
Each\ clay\ robot\ costs\ {INT} {   yy_clay_cost = atoi(yytext+22); }

Each\ obsidian\ robot\ costs\ {INT}\ ore\ and\ {INT}   {
    yy_obs_ore_cost = yytext[26] - '0';
    yy_obs_clay_cost= atoi(yytext+36);
}

Each\ geode\ robot\ costs\ {INT}\ ore\ and\ {INT}   {
    yy_geode_ore_cost = yytext[23] - '0';
    yy_geode_obs_cost = atoi(yytext+33);
}

\n  {
    yy_blueprint--;   
    blueprints[yy_blueprint].id = yy_blueprint+1;
    blueprints[yy_blueprint].ore_robot_ore_cost = yy_ore_cost;
    blueprints[yy_blueprint].clay_robot_ore_cost = yy_clay_cost;
    blueprints[yy_blueprint].obsidian_robot_ore_cost = yy_obs_ore_cost;
    blueprints[yy_blueprint].obsidian_robot_clay_cost = yy_obs_clay_cost;
    blueprints[yy_blueprint].geode_robot_clay_cost = yy_geode_ore_cost;
    blueprints[yy_blueprint].geode_robot_obsidian_cost = yy_geode_obs_cost;
}

\           |
:           |
ore.        |
clay.       |
obsidian.   {   }

<<EOF>>     {   return 0;   }
