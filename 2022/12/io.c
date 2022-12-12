#include "io.h"
#include "globals.h"
#include "funcs.h"
#include <stdlib.h>
#include <stdio.h>

void read_stdin(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;

    lineSize = getline(&line, &len, stdin);
    ncols = lineSize-1;

    textfile = malloc(MAXROWS * sizeof(char *));
    for(int i = 0; i < MAXROWS; i++)    textfile[i] = malloc(ncols * sizeof(char));

    while(lineSize > 1)
    {
        for(int i = 0; i < ncols; i++)  textfile[rowcounter][i] = line[i];
        rowcounter++;
        lineSize = getline(&line, &len, stdin);
    }

    free(line);
}

void fill(void)
{
    to_visit.head = NULL;
    starts.head = NULL;

    map = malloc(MAXROWS * sizeof(NODE *));
    for(int i = 0; i < rowcounter; i++)
    {
        map[i] = malloc(ncols * sizeof(NODE));

        for(int j = 0; j < ncols; j++){
            map[i][j].visited = false;
            map[i][j].distance = UINT16_MAX;
            map[i][j].first_neighbour = NULL;

            LISTITEM *add_to_visit = malloc(sizeof(LISTITEM));
            add_to_visit->next = to_visit.head;
            add_to_visit->this_node = &map[i][j];
            to_visit.head = add_to_visit;

            if(textfile[i][j] == 'S') {start = &map[i][j]; textfile[i][j]='a';}
            if(textfile[i][j] == 'E') {end = &map[i][j]; textfile[i][j]='z';}

            if(textfile[i][j] == 'a')
            {
                add_to_visit = malloc(sizeof(LISTITEM));
                add_to_visit->next = starts.head;
                add_to_visit->this_node = &map[i][j];
                starts.head = add_to_visit;

                nstarts++;
            }
        }
    }
    add_neighbours();   
}

void fill_reverse(void)
{
    to_visit.head = NULL;
    starts.head = NULL;

    map = malloc(MAXROWS * sizeof(NODE *));
    for(int i = 0; i < rowcounter; i++)
    {
        map[i] = malloc(ncols * sizeof(NODE));

        for(int j = 0; j < ncols; j++){
            map[i][j].visited = false;
            map[i][j].distance = UINT16_MAX;
            map[i][j].first_neighbour = NULL;

            LISTITEM *add_to_visit = malloc(sizeof(LISTITEM));
            add_to_visit->next = to_visit.head;
            add_to_visit->this_node = &map[i][j];
            to_visit.head = add_to_visit;

            if(textfile[i][j] == 'S') {end = &map[i][j]; textfile[i][j]='a';} // rev
            if(textfile[i][j] == 'E') {start = &map[i][j]; textfile[i][j]='z';} //rev

            if(textfile[i][j] == 'a')
            {
                add_to_visit = malloc(sizeof(LISTITEM));
                add_to_visit->next = starts.head;
                add_to_visit->this_node = &map[i][j];
                starts.head = add_to_visit;

                nstarts++;
            }

            textfile[i][j] = 'z' - (textfile[i][j]-'a');
        }
    }
    add_neighbours();   
}


void print_map(void)
{
    for(int i = 0; i < rowcounter; i++)
    {
        for(int j = 0; j < ncols; j++)
        {
            printf("[%p]%4d%4d%4c%12u",&map[i][j],i,j,textfile[i][j],map[i][j].distance);
            LISTITEM *current = map[i][j].first_neighbour;
            while(current)
            {
                printf("[%p]",current->this_node);
                current = current->next;
            }
            printf("\n");
        }
    }
}

void print_map_2(void)
{
    for(int i = 0; i < rowcounter; i++)
    {
        for(int j = 0; j < ncols; j++)
        {
            if(map[i][j].distance != UINT16_MAX) {
                printf("%3u",map[i][j].distance);
            } else  {
                printf(" * ");
            }
        }
        printf("\n");
    }
}
