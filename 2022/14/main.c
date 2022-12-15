#include "defs.h"
#include "lex.yy.h"

int last_x = INT16_MAX, last_y = INT16_MAX, curr_x = INT16_MAX, curr_y = INT16_MAX, escaped_y;
CELL **field;
bool pen_is_up = true;
const int n_x = 450, n_y = 175, offset_x = 250, offset_y = 0;

int main(int argc, char **argv)
{
    field = malloc(n_x * sizeof(CELL *));
    for(int i = 0; i < n_x; i++) {
        field[i] = malloc(n_y * sizeof(CELL));
        for(int j = 0; j < n_y; j++) {
            field[i][j] = EMPTY;
        }
    }

    yylex();

    int answer = drop_sand(false);

    for(int i = 0; i < n_x; i++) {
        for(int j = 0; j < n_y; j++) {
            if(field[i][j] == SAND) field[i][j] = EMPTY;
        }
    }

    int answer2 = drop_sand(true);

    for(int i = 0; i < n_x; i++) free(field[i]);
    free(field);

    printf("%8d%8d\n",answer,answer2);

    return 0;
}

int drop_sand(bool final_layer)
{
bool done = false;
    int sand_x, sand_y;
    int answer = 0;
    while(!done)
    {
        sand_x = 500 - offset_x;
        sand_y = 0 - offset_y;

        field[sand_x][sand_y] = SAND;

        while(true)
        {
            if(field[sand_x][sand_y+1] == EMPTY)
            {
                field[sand_x][sand_y] = EMPTY;
                sand_y++;
                field[sand_x][sand_y] = SAND;
            }
            else if(field[sand_x-1][sand_y+1] == EMPTY)
            {
                field[sand_x][sand_y] = EMPTY;
                sand_x--; sand_y++;
                field[sand_x][sand_y] = SAND;
            }
            else if(field[sand_x+1][sand_y+1] == EMPTY)
            {
                field[sand_x][sand_y] = EMPTY;
                sand_x++; sand_y++;
                field[sand_x][sand_y] = SAND;
            }
            else
            {
                if(sand_x == 500-offset_x && sand_y == 0-offset_y && final_layer) done=true;
                answer++;
                break;
            }

            if(sand_y == escaped_y)
            {
                if(final_layer) {
                    answer++;
                    break;
                } else {
                    field[sand_x][sand_y] = EMPTY;
                    done = true;
                    break;
                }
            }
        }
        //print_field();
    }
    //print_field();
    return answer;
}

void print_field(void)
{
    char toprint;
    for(int i = 0; i < n_y; i++){
        for(int j = 0; j < n_x; j++){
            if(field[j][i] == EMPTY) toprint = ' ';
            else if(field[j][i] == ROCK) toprint = '#';
            else toprint = 'o';
            printf("%c",toprint);
        }
        printf("\n");
    }
}

void draw(void)
{
    if(pen_is_up) return;

    if(curr_x == last_x){
        if(curr_y > last_y){
            for(int i = last_y; i <= curr_y; i++){
                field[curr_x-offset_x][i-offset_y] = ROCK;
            }
        } else {
            for(int i = curr_y; i <= last_y; i++){
                field[curr_x-offset_x][i-offset_y] = ROCK;
            }
        }
    }
    else if (curr_y == last_y)
    {
        if(curr_x > last_x){
            for(int i = last_x; i <= curr_x; i++){
                field[i-offset_x][curr_y-offset_y] = ROCK;
            }
        } else {
            for(int i = curr_x; i <= last_x; i++){
                field[i-offset_x][curr_y-offset_y] = ROCK;
            }
        }

    }
    return;
}