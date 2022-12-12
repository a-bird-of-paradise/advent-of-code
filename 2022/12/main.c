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

}

int main(int argc, char *argv[])
{
    read_stdin();
    fill_reverse();
    walk(start);
    printf("%8u\n",end->distance);

    LISTITEM *current = starts.head;
    unsigned answer = UINT16_MAX;

    for(int i = 0; i < nstarts; i++)
    {
        answer = current->this_node->distance < answer ? current->this_node->distance : answer;
        current = current->next;
    }

    printf("%8u\n",answer);

    free_all();
}