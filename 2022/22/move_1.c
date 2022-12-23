#include "22.h"
#include <stdio.h>
#include "lex.yy.h"

void move_1(void) 
{

    POSITION location;
    location.f = RIGHT;
    location.y = 0;
    int counter=0; 
    while(field[0][counter]!='.') counter++; 
    location.x = counter;
    
    int next_x, next_y, orig_x, orig_y; 
    bool noisy = true, crossing_edge = false;

    while((counter = yylex()))
    {
        if(noisy) printf("counter: %d\n",counter);
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

                if(noisy) printf("%5d %5d:",orig_x,orig_y);
                if(noisy) printf("%4d %4d\n",nodes[orig_y][orig_x].right->x,nodes[orig_y][orig_x].right->y);

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
                    case '.':   location.x = next_x; location.y = next_y; counter--; 
                                    if(abs(orig_x-next_x) > 1 || abs(orig_y-next_y)>1)
                                        location.f = nodes[next_y][next_x].post_edge_facing;
                                break;
                    case '#':   counter = 0; break;
                    case ' ':   printf("oh no\n"); break;
                    default:    break;

                

                }
            }
        }
    }

    if(noisy) printf("%10d %10d %10d\n",location.x,location.y,location.f);
    printf("%20d\n",1000 * (location.y+1) + 4*(location.x+1) + location.f);
}
