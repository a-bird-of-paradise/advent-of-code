#include "22.h"
#include <stdlib.h>

void init_part_1(void)
{
    int counter;
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
            nodes[i][j].is_edge = false; // part 1 don't care
            nodes[i][j].post_edge_facing = RIGHT; // part 1 don't care
        }
    }
}