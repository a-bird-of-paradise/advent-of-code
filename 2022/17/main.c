#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct point {int x; int y;} POINT;
#define FIELD_LEN 100000

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    long long ymax = 0, ymax_last = 0;
    int rock_counter = 0;
    int wind_id = 0, next_wind_id;

    nread = getline(&line, &len, stdin);
    nread--;//don't care about \n

    // for simming
    char this_jet;
    
    char (*field)[FIELD_LEN] = malloc(sizeof(char[7][FIELD_LEN]));
    int dy[2022] = {0};

    for(int i = 0; i < 7; i++) 
        for(int j = 0; j < FIELD_LEN; j++)
            field[i][j]=' ';

    POINT moving[5] = {0};

    bool blocked_left, blocked_right, settle = false, next_rock, breakout;

    while(rock_counter<2022)
    {
        switch(rock_counter % 5) {
            case 0: // horiz 4
                moving[0].x = 2; moving[0].y=ymax+3;
                moving[1].x = 3; moving[1].y=ymax+3;
                moving[2].x = 4; moving[2].y=ymax+3;
                moving[3].x = 5; moving[3].y=ymax+3;
                moving[4].x = INT16_MAX; moving[4].y=INT16_MAX;
                break;
            case 1: // cross
                moving[0].x = 2; moving[0].y=ymax+3 + 1;
                moving[1].x = 3; moving[1].y=ymax+3 + 1;
                moving[2].x = 4; moving[2].y=ymax+3 + 1;
                moving[3].x = 3; moving[3].y=ymax+3 + 2;
                moving[4].x = 3; moving[4].y=ymax+3 + 0;
                break;
            case 2: // reversed L 3x3
                moving[0].x = 2; moving[0].y=ymax+3;
                moving[1].x = 3; moving[1].y=ymax+3;
                moving[2].x = 4; moving[2].y=ymax+3;
                moving[3].x = 4; moving[3].y=ymax+3 + 1;
                moving[4].x = 4; moving[4].y=ymax+3 + 2;
                break;
            case 3: // vert 4
                moving[0].x = 2; moving[0].y=ymax+3;
                moving[1].x = 2; moving[1].y=ymax+3 + 1;
                moving[2].x = 2; moving[2].y=ymax+3 + 2;
                moving[3].x = 2; moving[3].y=ymax+3 + 3;
                moving[4].x = INT16_MAX; moving[4].y=INT16_MAX;
                break;
            case 4: // block
                moving[0].x = 2; moving[0].y=ymax+3;
                moving[1].x = 2; moving[1].y=ymax+3 + 1;
                moving[2].x = 3; moving[2].y=ymax+3;
                moving[3].x = 3; moving[3].y=ymax+3 + 1;
                moving[4].x = INT16_MAX; moving[4].y=INT16_MAX;
                break;
            default: // never
                break;
        }

        next_rock = false;

        while(!next_rock) // seek a collision
        {
            this_jet = line[wind_id];

            blocked_left = false;
            blocked_right = false;

            if(rock_counter<5 && false) {
                for(int i = 30; i >= 0; i--)
                {
                    printf("|");
                    for(int j = 0; j < 7; j++){
                        char toprint = field[j][i];
                        for(int k = 0; k < 5; k++) {
                            if(moving[k].x== j && moving[k].y == i){
                                toprint = '@';
                            } 
                        }
                        printf("%c",toprint);
                    }
                    printf("|\n");
                }
                printf("---------\n");
            }

            // can i be pushed?
            for(int i = 0; i < 5; i++)
            {   
                if(moving[i].x == INT16_MAX) continue;
                if(moving[i].x == 0) blocked_left = true;
                if(moving[i].x == 6) blocked_right = true;
                if(!blocked_left && field[moving[i].x-1][moving[i].y] == '#') blocked_left = true;
                if(!blocked_right && field[moving[i].x+1][moving[i].y] == '#') blocked_right = true;
            }
            // be pushed
            if(!blocked_left && this_jet == '<'){
                for(int i = 0; i < 5; i++) {
                    if(moving[i].x != INT16_MAX) {
                        moving[i].x--;
                    }

                }
            } else if(!blocked_right && this_jet == '>'){
                for(int i = 0; i < 5; i++) {
                    if(moving[i].x != INT16_MAX) {
                        moving[i].x++;
                    }
                }
            }

            // I've been pushed; am i stopping or dropping?

            settle = false;

            for(int i = 0; i < 5; i++) {
                if(moving[i].x != INT16_MAX) {
                    if(moving[i].y == 0 || // rock bottom
                        field[moving[i].x][moving[i].y-1] == '#') {
                            settle = true;
                     }
                }
            }

            if(settle) {
                for(int i = 0; i < 5; i++) {
                    if(moving[i].x != INT16_MAX) {
                        field[moving[i].x][moving[i].y] = '#';
                    }
                }
                ymax_last=ymax;
                ymax=0;
                for(int i = ymax_last-1; i < ymax_last+5; i++) {
                    if(i<0) continue;
                    for(int j = 0; j < 7; j++) {
                        if(field[j][i]=='#' && i > ymax) {
                            ymax = i;
                        }
                    }
                }
                ymax++;
                next_rock = true;
                settle = false;
            } else {
                for(int i = 0; i < 5; i++) {
                    if(moving[i].x != INT16_MAX) {
                        moving[i].y--;
                    }
                }
            }

            wind_id++;
            wind_id %= nread;
        }

        dy[rock_counter] = ymax - ymax_last;

        rock_counter++;
        if(rock_counter<5 && false) {
            for(int i = 30; i >= 0; i--)
            {
                printf("|");
                for(int j = 0; j < 7; j++) printf("%c",field[j][i]);
                printf("|\n");
            }
            printf("---------\n");
        }
        //printf("%lld",ymax-ymax_last);
        //if((rock_counter +   35 - 15) %   35 == 0) printf("\n"); //test
        //if((rock_counter + 1735 - 94) % 1735 == 0) printf("\n"); //real
    }

    long long head = 0, repeat = 0, tail_length = 0, tail = 0, total = 0, nrepeats=0;

    long long head_length, cycle_length, nrocks, n_cycles;

    head_length = 27; cycle_length = 35; nrocks = 2022;
    
    for(int i = 0; i < head_length; i++) head += dy[i];
    for(int i = head_length; i < head_length + cycle_length; i++) repeat += dy[i];
    nrepeats = (nrocks - head_length) / cycle_length;
    tail_length = nrocks - ((nrepeats+0) * cycle_length) - head_length;
    for(int i = 0; i < tail_length; i++) tail += dy[head_length + i];

    total = head + (nrepeats * repeat) + tail;
    printf("%14lld 2022 rocks, test data 3068\n", total); // test

// 2022   
    head = 0; repeat = 0; tail_length = 0; tail = 0; total = 0; nrepeats=0; 
    head_length = 94; cycle_length = 1735; nrocks = 2022;
    for(int i = 0; i < head_length; i++) head += dy[i];
    for(int i = head_length; i < head_length + cycle_length; i++) repeat += dy[i];
    nrepeats = (nrocks - head_length) / cycle_length;
    tail_length = nrocks - ((nrepeats+0) * cycle_length) - head_length;
    for(int i = 0; i < tail_length; i++) tail += dy[head_length + i];

    total = head + (nrepeats * repeat) + tail;
    printf("%14lld 2022 rocks, real data 3179\n", total); // test

// 1tn   
    head = 0; repeat = 0; tail_length = 0; tail = 0; total = 0; nrepeats=0; 
    head_length = 27; cycle_length = 35; nrocks = 1000000000000;
    for(int i = 0; i < head_length; i++) head += dy[i];
    for(int i = head_length; i < head_length + cycle_length; i++) repeat += dy[i];
    nrepeats = (nrocks - head_length) / cycle_length;
    tail_length = nrocks - ((nrepeats+0) * cycle_length) - head_length;
    for(int i = 0; i < tail_length; i++) tail += dy[head_length + i];

    total = head + (nrepeats * repeat) + tail;
    printf("%14lld 1tn rocks, test data 1514285714288\n", total); // test

// 1tn   
    head = 0; repeat = 0; tail_length = 0; tail = 0; total = 0; nrepeats=0; 
    head_length = 94; cycle_length = 1735; nrocks = 1000000000000;
    for(int i = 0; i < head_length; i++) head += dy[i];
    for(int i = head_length; i < head_length + cycle_length; i++) repeat += dy[i];
    nrepeats = (nrocks - head_length) / cycle_length;
    tail_length = nrocks - ((nrepeats+0) * cycle_length) - head_length;
    for(int i = 0; i < tail_length; i++) tail += dy[head_length + i];

    total = head + (nrepeats * repeat) + tail;
    printf("%14lld 1tn rocks, real data 1567723342929\n", total); // test

    free(line);
    free(field);
    exit(EXIT_SUCCESS);
}