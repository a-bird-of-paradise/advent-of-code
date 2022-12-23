#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.h"
#include "22.h"

char field [200][201] = {0}; // [row][col] [y][x]
char *line = NULL, *line2 = NULL;
size_t linecap = 0;
ssize_t linelen;
YY_BUFFER_STATE hndl;

NODE nodes[200][200] = {0};

int mod (const int a, const int b) { return (a%b + b)%b; }

int main(int argc, char **argv)
{
    int counter = 0;

    while((linelen = getline(&line, &linecap, stdin)) > 1)
        strncpy(field[counter++],line,linelen-1);

    linelen = getline(&line, &linecap, stdin);
    line2 = strdup(line); // flex pollutes the buffer so take a copy for part 2

    hndl = yy_scan_string(line);

    for(int i = 0; i <200; i++)
        for(int j = 0; j < 200; j++)
            if(field[i][j] == 0)
                field[i][j]=' ';
    
    for(int i = 0; i <200; i++) field[i][200]=0;

    #pragma region init_part_1

    // init nodes for part 1
    for(int i = 0; i < 200; i++){
        for(int j = 0; j < 200; j++){
            if(field[i][j] == '.' || field[i][j] == '#') {
                nodes[i][j].x = j; 
                nodes[i][j].y = i;
                nodes[i][j].value = field[i][j];
                // who is up
                for(counter = 1; counter < 200; counter++){
                    if(field[mod(nodes[i][j].y-counter,200)][nodes[i][j].x] != ' ') {
                        break;
                    }
                }
                nodes[i][j].up = &nodes[mod(nodes[i][j].y-counter,200)][nodes[i][j].x];

                // who is down
                for(counter = 1; counter < 200; counter++){
                    if(field[mod(nodes[i][j].y+counter,200)][nodes[i][j].x] != ' ') {
                        break;
                    }
                }
                nodes[i][j].down = &nodes[mod(nodes[i][j].y+counter,200)][nodes[i][j].x];

                // who is left
                for(counter = 1; counter < 200; counter++){
                    if(field[nodes[i][j].y][mod(nodes[i][j].x-counter,200)] != ' ') {
                        break;
                    }
                }
                nodes[i][j].left = &nodes[nodes[i][j].y][mod(nodes[i][j].x-counter,200)];

                // who is right
                for(counter = 1; counter < 200; counter++){
                    if(field[nodes[i][j].y][mod(nodes[i][j].x+counter,200)] != ' ') {
                        break;
                    }
                }
                nodes[i][j].right = &nodes[nodes[i][j].y][mod(nodes[i][j].x+counter,200)];

            } else {
                nodes[i][j].x = INT16_MAX; 
                nodes[i][j].y = INT16_MAX;
                nodes[i][j].value = ' ';
            }
        }
    }

    #pragma endregion init_part_1

    #pragma region move_part_1

    POSITION location;
    location.f = RIGHT;
    location.y = 0;
    counter=0; while(field[0][counter]!='.') counter++; location.x = counter;
    
    int next_x, next_y, orig_x, orig_y; 

    while((counter = yylex()))
    {
        printf("counter: %d\n",counter);
        switch(counter){

            case -2:   // R
                switch(location.f) {
                    case UP: location.f = RIGHT; break;
                    case DOWN: location.f = LEFT; break;
                    case LEFT: location.f = UP; break;
                    case RIGHT: location.f = DOWN; break;
                    default:    break;
                }
            break;

            case -1:    // L
                switch(location.f) {
                    case UP: location.f = LEFT; break;
                    case DOWN: location.f = RIGHT; break;
                    case LEFT: location.f = DOWN; break;
                    case RIGHT: location.f = UP; break;
                    default:    break;
                }
            break;

            case 0:     break;

            default:   
            while( counter > 0) {
                orig_x = location.x;
                orig_y = location.y;

                printf("%5d %5d:",orig_x,orig_y);
                printf("%4d %4d\n",nodes[orig_y][orig_x].right->x,nodes[orig_y][orig_x].right->y);

                switch(location.f) {
                    case UP:    next_y = nodes[orig_y][orig_x].up->y; 
                                next_x = nodes[orig_y][orig_x].up->x; break;
                    case DOWN:  next_y = nodes[orig_y][orig_x].down->y; 
                                next_x = nodes[orig_y][orig_x].down->x; break;
                    case LEFT:  next_y = nodes[orig_y][orig_x].left->y; 
                                next_x = nodes[orig_y][orig_x].left->x; break;
                    case RIGHT: next_y = nodes[orig_y][orig_x].right->y; 
                                next_x = nodes[orig_y][orig_x].right->x; break;
                    default:    break;
                }

                switch(field[next_y][next_x]) {
                    case '.':   location.x = next_x; location.y = next_y; counter--; break;
                    case '#':   counter = 0; break;
                    case ' ':   printf("oh no\n"); break;
                    default:    break;
                }
            }
        }
    }

    printf("%10d %10d %10d\n",location.x,location.y,location.f);
    printf("%20d\n",1000 * (location.y+1) + 4*(location.x+1) + location.f);

    #pragma endregion move_part_1

    yy_delete_buffer(hndl);
    free(line);
    free(line2);

    return 0;
}