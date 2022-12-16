#include "valves.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void compute_distances(void)
{

    for(int i = 0; i < MAX_VALVES; i++)
        for(int j = 0; j < MAX_VALVES; j++)
            distances[i][j] = INT16_MAX;

    EDGE *this_edge = edges.head;
    while(this_edge)
    {
            distances[this_edge->from->index][this_edge->to->index]=1;
            distances[this_edge->to->index][this_edge->from->index]=1;
            this_edge = this_edge->next;
    }
    for(int i = 0; i < n_valves; i++)
        distances[i][i]=0;

    for(int k = 0; k < n_valves; k++)
        for(int i = 0; i < n_valves; i++)
            for(int j = 0; j < n_valves; j++)
                if(distances[i][j] > distances[i][k] + distances[k][j])
                    distances[i][j] = distances[i][k] + distances[k][j];
}

void set_indices(void)
{
    for(int i = 0; i < MAX_VALVES; i++)
        valves[i].index = i;
}

void add_neighbours(void)
{
    EDGE *current = edges.head;
    VALVE *from, *to;
    VALVE_LIST_ITEM *from_neighbour, *to_neighbour;

    while(current)
    {
        from = current->from; to = current->to;

        from_neighbour = from->neighbours.head;
        to_neighbour = to->neighbours.head;
        
        if(!from_neighbour) {
            from->neighbours.head = malloc(sizeof(VALVE_LIST_ITEM));
            from->neighbours.head->next = NULL;
            from->neighbours.head->item = to;
        } else {
            while(from_neighbour->next) from_neighbour=from_neighbour->next;
            from_neighbour->next = malloc(sizeof(VALVE_LIST_ITEM));
            from_neighbour->next->item = to;
            from_neighbour->next->next = NULL;
        }
        
        if(!to_neighbour) {
            to->neighbours.head = malloc(sizeof(VALVE_LIST_ITEM));
            to->neighbours.head->next = NULL;
            to->neighbours.head->item = from;
        } else {
            while(to_neighbour->next) to_neighbour=to_neighbour->next;
            to_neighbour->next = malloc(sizeof(VALVE_LIST_ITEM));
            to_neighbour->next->item = from;
            to_neighbour->next->next = NULL;
        }
        current = current->next; 
    }
}

void repair(void)
{
    EDGE *this_edge = edges.head;
    while(this_edge)
    {
        this_edge->from = find_valve(this_edge->from_label);
        this_edge->to = find_valve(this_edge->to_label);
        this_edge = this_edge->next;
    }
}

int valve_cmp(const void* a, const void* b)
{
    // NULLs go to the back
    if((*(VALVE *)a).label[0] == 0 && (*(VALVE *)b).label[0] != 0) return 1;
    if((*(VALVE *)a).label[0] != 0 && (*(VALVE *)b).label[0] == 0) return -1;

    return strncmp((*(VALVE *)a).label,(*(VALVE *)b).label,2);
}

VALVE *find_valve(const char label[2]){

    char this_label[2];

    for(int i = 0; i < MAX_VALVES; i++)
    {
        strncpy(this_label,valves[i].label,2);
        if(this_label[0] == label[0] && this_label[1] == label[1]) {
            return &valves[i];
        }
    }

    // not found, so create it 

    for(int i = 0; i < MAX_VALVES; i++)
    {
        if(valves[i].label[0] == 0 && valves[i].label[1] == 0) {
            strncpy(valves[i].label,label,2);
            return &valves[i];
        }
    }

    return NULL;
}

EDGE *add_edge(VALVE *from, VALVE *to)
{
    // edges stored so that "from" is lexicographically before "to" 
    // as this isn't a directed graph 

    if(to->label[0] < from->label[0]){
        VALVE *temp = to;
        to = from;
        from = temp;
    }
    else if(to->label[0] == from->label[0] && to->label[1] < from->label[1]){
        VALVE *temp = to;
        to = from;
        from = temp;
    }

    EDGE *current = edges.head;
    if(!current) { // first
        edges.head = malloc(sizeof(EDGE));
        edges.head->next = NULL;
        current = edges.head;
    } else {

        if(!strncmp(current->from_label,from->label,2) &&
            !strncmp(current->to_label,to->label,2)) {
                return current;
            }

        while(current->next) {
            current = current->next;

            if(!strncmp(current->from_label,from->label,2) &&
                !strncmp(current->to_label,to->label,2)) {
                    return current;
                }

        }

        current->next = malloc(sizeof(EDGE));
        current = current->next;
        current->next = NULL;

    }

    current->from = from;
    current->to = to;

    strncpy(current->from_label,from->label,2);
    strncpy(current->to_label,to->label,2);

    return current;   
}

void valve_info(void)
{
    for(int i = 0; i < n_valves; i++)
    {

        printf("%.*s:%4d:",2,valves[i].label,valves[i].flow_rate);

        VALVE_LIST_ITEM *current_neighbour = valves[i].neighbours.head;
        while(current_neighbour) {
            printf(" %.*s",2,current_neighbour->item->label);
            if(current_neighbour->next) printf(",");
            current_neighbour = current_neighbour->next;
        }
        printf("\n");
    }

    for(int i = 0; i < n_valves; i++){
        for(int j = 0; j < n_valves; j++) {
            printf("%3d",distances[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n_valves; i++){
        if(valves[i].flow_rate == 0) continue;
        for(int j = 0; j < n_valves; j++) {
            if(valves[j].flow_rate == 0) continue;
            printf("%3d",distances[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < used_valves; i++)
        printf("%.*s ",2,valves[interesting[i+1]].label);
    printf("\n");

}

