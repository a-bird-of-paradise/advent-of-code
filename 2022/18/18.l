%option 8bit yylineno noyywrap fast align batch nomain

%{
#include <stdbool.h>
#define WIDTH 25
typedef enum content { VOID, ROCK, AIR} CONTENT;
CONTENT arena[WIDTH][WIDTH][WIDTH] = {VOID};

void fill_with_rock(const int x, const int y, const int z);
long track_surface_area(const int x, const int y, const int z, bool only_total);
void flood_fill(const int x, const int y, const int z, CONTENT with);
long open_surfaces(void);

int yy_x, yy_y, yy_z;
%}

INT [-+]?[0-9]+

%%

^{INT}/,    {   yy_x = atoi(yytext+0);          }
,{INT}/,    {   yy_y = atoi(yytext+1);          }
,{INT}$     {   yy_z = atoi(yytext+1);          }
\n          {   fill_with_rock(yy_x,yy_y,yy_z); }
<<EOF>>     {   return    0;                    }

%%

long open_surfaces(void)
{
    long answer = 0;

    for(int x = 0; x < WIDTH; x++){
        for(int y=0; y < WIDTH; y++) {
            for(int z = 0; z < WIDTH; z++){
                if(arena[x][y][z] == ROCK){
                    if(x>0      && arena[x-1][y][z] == AIR) answer++;
                    if(x<WIDTH-1&& arena[x+1][y][z] == AIR) answer++;
                    if(y>0      && arena[x][y-1][z] == AIR) answer++;
                    if(y<WIDTH-1&& arena[x][y+1][z] == AIR) answer++;
                    if(z>0      && arena[x][y][z-1] == AIR) answer++;
                    if(z<WIDTH-1&& arena[x][y][z+1] == AIR) answer++;

                    if(x == 0 || x == WIDTH) answer++;
                    if(y == 0 || y == WIDTH) answer++;
                    if(z == 0 || z == WIDTH) answer++;
                }
            }
        }
    }
    return answer;
}

void flood_fill(const int x, const int y, const int z, CONTENT with)
{
    // what am i? 
    CONTENT myself = arena[x][y][z];
    // fill my neighbours if they are like me
    arena[x][y][z] = with;

    if(x>0      && arena[x-1][y][z] == myself) flood_fill(x-1,y,z,with);
    if(x<WIDTH-1&& arena[x+1][y][z] == myself) flood_fill(x+1,y,z,with);
    if(y>0      && arena[x][y-1][z] == myself) flood_fill(x,y-1,z,with);
    if(y<WIDTH-1&& arena[x][y+1][z] == myself) flood_fill(x,y+1,z,with);
    if(z>0      && arena[x][y][z-1] == myself) flood_fill(x,y,z-1,with);
    if(z<WIDTH-1&& arena[x][y][z+1] == myself) flood_fill(x,y,z+1,with);
}
void fill_with_rock(const int x, const int y, const int z)
{
    arena[x][y][z] = ROCK;
    track_surface_area(x,y,z,false);
}

long track_surface_area(const int x, const int y, const int z, bool only_total)
{
    static long surface_area = 0;
    if(only_total) return surface_area;
    // add all new surfaces...
    surface_area += 6;
    // and deduct any neighbours
    if(x>0      && arena[x-1][y][z] == ROCK) surface_area -= 2;
    if(x<WIDTH  && arena[x+1][y][z] == ROCK) surface_area -= 2;
    if(y>0      && arena[x][y-1][z] == ROCK) surface_area -= 2;
    if(y<WIDTH  && arena[x][y+1][z] == ROCK) surface_area -= 2;
    if(z>0      && arena[x][y][z-1] == ROCK) surface_area -= 2;
    if(z<WIDTH  && arena[x][y][z+1] == ROCK) surface_area -= 2;
    return surface_area;
}

int main(int argc, char **argv)
{
    yylex();
    printf("%8ld\n",track_surface_area(0,0,0,true));
    flood_fill(WIDTH-1,WIDTH-1,WIDTH-1,AIR);
    printf("%8ld\n",open_surfaces());
    return 0;
}