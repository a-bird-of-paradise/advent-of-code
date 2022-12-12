#include "funcs.h"
#include "globals.h"
#include <stdlib.h>

void visit(NODE *current)
{
    LISTITEM *current_neighbour = current->first_neighbour;
    LISTITEM *current_unvis, *prev_unvis;

    while(current_neighbour)
    {
        current_neighbour->this_node->distance = 
            current->distance+1 < current_neighbour->this_node->distance ?
                current->distance + 1 : current_neighbour->this_node->distance;

        current_neighbour = current_neighbour->next;
    }

    current->visited = true;

    current_unvis = to_visit.head;
    prev_unvis = NULL;

    // have we exhausted all nodes?

    if(current_unvis)
    {
        // seek to this node
        while(current_unvis->this_node != current)
        {
            prev_unvis = current_unvis;
            current_unvis = current_unvis->next;
        }

        // are we still at the head?

        if(!prev_unvis)
        {
            to_visit.head = current_unvis->next;
        }
        else
        {
            prev_unvis->next = current_unvis->next;
        }

        free(current_unvis);
    }
}

void walk(NODE *towalk)
{
    towalk->distance=0;
    visit(towalk);
    while(to_visit.head)
    {
        LISTITEM *smallest, *current;
        unsigned mindistance = UINT16_MAX;

        current = to_visit.head;
        smallest = NULL;

        while(current)
        {
            if(current->this_node->distance <= mindistance)
            {
                smallest = current;
                mindistance = smallest->this_node->distance;
            }
            current = current->next;
        }
        if(!smallest) break;
        visit(smallest->this_node);
    }
}

void reset_nodes(void)
{

    LISTITEM *visitor = to_visit.head, *next_l;

    while(visitor)
    {
        next_l = visitor->next;
        free(visitor);
        visitor = next_l;
    }

    to_visit.head = NULL;

    for(int i = 0; i < rowcounter; i++)
    {
        for(int j = 0; j < ncols; j++)
        {
            map[i][j].visited = false;
            map[i][j].distance = UINT16_MAX;

            visitor = malloc(sizeof(LISTITEM));
            visitor->next = to_visit.head;
            visitor->this_node = &map[i][j];
            to_visit.head = visitor;

        }
    }
}

void add_neighbour(NODE *addee, NODE *new_neighbour)
{
    LISTITEM *current = addee->first_neighbour, *new_item;

    new_item = malloc(sizeof(LISTITEM));
    new_item->this_node = new_neighbour;
    new_item->next = NULL;

    if(current){
        while(current->next) current = current->next;
        current->next = new_item;
    }
    else
    {
        addee->first_neighbour = new_item;
    }
}

void add_neighbours(void)
{
for(int i = 0; i < rowcounter; i++)
    {
        for(int j = 0; j < ncols; j++)
        {
            // up neighbour
            if(i>0){
                if(textfile[i][j]+1 >= textfile[i-1][j])
                    add_neighbour(&map[i][j], &map[i-1][j]);
            }
            // down neighbour
            if(i<rowcounter-1){
                if(textfile[i][j]+1 >= textfile[i+1][j])
                    add_neighbour(&map[i][j], &map[i+1][j]);
            }
            // left neighbour
            if(j>0){
                if(textfile[i][j]+1 >= textfile[i][j-1])
                    add_neighbour(&map[i][j], &map[i][j-1]);
            }
            // right neighbour
            if(j<ncols-1){
                if(textfile[i][j]+1 >= textfile[i][j+1])
                    add_neighbour(&map[i][j], &map[i][j+1]);
            }
        }
    }
}