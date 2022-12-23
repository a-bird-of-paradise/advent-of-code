#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.h"
#include "22.h"

char field [200][201] = {0}; // [row][col] [y][x]
NODE nodes[200][200] = {0};

int mod(const int a, const int b) { return (a%b + b)%b; }

int abs(const int x) { return x > 0 ? x : -x; }

void print_nodes(void)
{
    printf("   ");
    for(int i = 0; i < 16; i++) printf("%2d",i%100);
    printf("\n");

    for(int i = 0; i < 12; i++){
        printf("%2d ",i);
        for(int j = 0; j < 16; j++) {
            printf(" %c",nodes[i][j].value);
        }
        printf("\n");
    }
}

void print_facing(FACING x)
{
    char toprint = '*';
    switch(x){
        case DOWN:  toprint = 'D'; break;
        case UP:    toprint = 'U'; break;
        case LEFT:  toprint = 'L'; break;
        case RIGHT: toprint = 'R'; break;
        default:    break;
    }
    printf("%c",toprint);
}

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    int counter = 0;

    while((linelen = getline(&line, &linecap, stdin)) > 1)
        strncpy(field[counter++],line,linelen-1);

    linelen = getline(&line, &linecap, stdin);

    for(int i = 0; i <200; i++)
        for(int j = 0; j < 200; j++)
            if(field[i][j] == 0)
                field[i][j]=' ';
    
    for(int i = 0; i <200; i++) field[i][200]=0;

    YY_BUFFER_STATE hndl;

    hndl = yy_scan_string(line);
    init_part_1();
    move_1();
    yy_delete_buffer(hndl);

    hndl = yy_scan_string(line);
    if(strlen(line)<100) init_part_2_test();
    else init_part_2_real();
    move_1();
    yy_delete_buffer(hndl);

    free(line);

    return 0;
}