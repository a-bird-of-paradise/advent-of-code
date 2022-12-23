#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GRID 150
#define OFFSET 25

char field[GRID][GRID]; // row col ; y x

void print_grid(void)
{
    for(int i = 0; i < GRID; i++) {
        for(int j = 0; j < GRID; j++) {
            printf("%c",field[i][j] );
        }
        printf("\n");
    }
}

void print_part_1(void)
{
    int min_x = INT16_MAX, min_y = INT16_MAX, max_x = INT16_MIN, max_y = INT16_MIN;
    int n_elves = 0;

    for(int i = 1; i < GRID - 1; i++) {
        for(int j = 1; j < GRID - 1; j++) {
            if(field[i][j] == '#') {
                min_x = j < min_x ? j : min_x;
                min_y = i < min_y ? i : min_y;
                max_x = j > max_x ? j : max_x;
                max_y = i > max_y ? i : max_y;
                n_elves++;
            }
        }
    }

    printf("%4d %4d %4d %4d %4d\n", min_x, max_x, min_y, max_y, n_elves);
    printf("%4d\n", ( max_x - min_x + 1) * (max_y - min_y + 1) - n_elves);
}

int main(int argc, char **argv)
{

    for(int i = 0; i < GRID; i++) 
        for(int j = 0; j < GRID; j++) 
            field[j][i]='.';

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    int counter = 0;

    while((linelen = getline(&line, &linecap, stdin)) > 0)
        strncpy(&field[counter++ +OFFSET][OFFSET],line,linelen-1);

    bool proposed;
    int n_moved = INT16_MAX;

    int round = 0;

    while(n_moved)
    {
        // part 1: consider moves
        for(int i = 1; i < GRID - 1; i++) {
            for(int j = 1; j < GRID - 1; j++)
            {
                if(field[i][j] == '#'){

                    // neighbours?

                    if(field[i][j-1] != '#' && field[i][j+1] != '#' 
                        && field[i-1][j-1] != '#' && field[i-1][j] != '#' && field[i-1][j+1] != '#'
                        && field[i+1][j-1] != '#' && field[i+1][j] != '#' && field[i+1][j+1] != '#') 
                        continue;

                    proposed = false;

                    for(int k = 0; k < 4; k++){
                        switch( (round + k) % 4) {
                            case 0: // N
                                if(field[i-1][j] != '#' && field[i-1][j+1] != '#' && field[i-1][j-1] != '#') {
                                        if(field[i-1][j] != '.') {field[i-1][j] = 'X';}
                                        else {field[i-1][j] = 'N';} 
                                        proposed = true;
                                    }
                                    break;
                            case 1: // S
                                if(field[i+1][j] != '#' && field[i+1][j+1] != '#' && field[i+1][j-1] != '#') {
                                        if(field[i+1][j] != '.') {field[i+1][j] = 'X';}
                                        else {field[i+1][j]= 'S';} 
                                        proposed = true;
                                    }
                                    break;
                            case 2: // W
                                if(field[i-1][j-1] != '#' && field[i][j-1] != '#' && field[i+1][j-1] != '#') {
                                        if(field[i][j-1] != '.') {field[i][j-1] = 'X';} 
                                        else {field[i][j-1]= 'W';}
                                        proposed = true;
                                    }
                                    break;
                            case 3: // E
                                if(field[i-1][j+1] != '#' && field[i][j+1] != '#' && field[i+1][j+1] != '#') {
                                        if(field[i][j+1] != '.') {field[i][j+1] = 'X';}
                                        else {field[i][j+1]= 'E';}
                                        proposed = true;
                                    }
                                    break;
                            default:    break;
                        }
                        if(proposed) break;
                    }
                }
            }
        }
    
        // part 2: do moves

        n_moved = 0;

        for(int i = 1; i < GRID - 1; i++) {
            for(int j = 1; j < GRID - 1; j++) {
                if(field[i][j] == '#') {

                    if(field[i-1][j] == 'N') {
                        field[i-1][j]='#'; field[i][j] = '.'; n_moved++;
                    }
                    else if(field[i+1][j] == 'S') {
                        field[i+1][j]='#'; field[i][j] = '.'; n_moved++;
                    }
                    else if(field[i][j-1] == 'W') {
                        field[i][j-1]='#'; field[i][j] = '.'; n_moved++;
                    }
                    else if(field[i][j+1] == 'E') {
                        field[i][j+1]='#'; field[i][j] = '.'; n_moved++;
                    }
                    
                } else if (field[i][j] == 'X') {
                    field[i][j]='.';
                }
            }
        }

        round++;
        if(round == 10) print_part_1();

    }

    printf("%4d\n",round);

    free(line);

}