#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "globals.h"
#include "funcs.h"
#include "io.h"

void free_all(void)
{
    for(int i = 0; i < MAXROWS; i++) free(textfile[i]);
    free(textfile);

    LISTITEM *current_neighbour, *next_neighbour;

    for(int i = 0; i < rowcounter; i++)
    {
        for(int j = 0; j < ncols; j++)
        {
            current_neighbour = map[i][j].first_neighbour;
            while(current_neighbour)
            {
                next_neighbour = current_neighbour->next;
                free(current_neighbour);
                current_neighbour = next_neighbour;
            }
        }
        free(map[i]);
    }
    free(map);

    LISTITEM *to_visit_node = to_visit.head, *next_to_visit;

    while(to_visit_node)
    {
        next_to_visit = to_visit_node->next;
        free(to_visit_node);
        to_visit_node = next_to_visit;
    }

    to_visit_node = starts.head;

    while(to_visit_node)
    {
        next_to_visit = to_visit_node->next;
        free(to_visit_node);
        to_visit_node = next_to_visit;
    }

    if(startdistances) free(startdistances);
}

int cmpfunc(const void *a, const void *b)
{
   const unsigned *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int main(int argc, char *argv[])
{
    read_stdin();
    fill();
    walk(start);
    printf("%8u\n",end->distance);

    printf("%8d\n",nstarts);

    startdistances = malloc(nstarts * sizeof(unsigned));
    int startcounter = 0;

    LISTITEM *this_start = starts.head;

    while(this_start)
    {
        reset_nodes();
        walk(this_start->this_node);
        startdistances[startcounter] = end->distance;
        printf("%4d%8u\n",startcounter,startdistances[startcounter]);
        startcounter++;
        
        this_start = this_start->next;
        
        //if(startdistances[startcounter] != UINT16_MAX) print_map_2();
    }

    qsort(startdistances,nstarts,sizeof(unsigned),cmpfunc);

    printf("%8u\n",startdistances[0]);

    free_all();
}