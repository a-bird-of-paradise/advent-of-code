#pragma once

#define MAX_VALVES 58

#define N_PATHS 10000000
#define PATH_WIDTH 16

typedef struct valve_list_item {
    struct valve *item;
    struct valve_list_item *next;
} VALVE_LIST_ITEM;

typedef struct valve_list {
    VALVE_LIST_ITEM *head;
} VALVE_LIST;

typedef struct valve {
    int flow_rate;
    char label[2];
    VALVE_LIST neighbours;
    int index;
} VALVE;

typedef struct edge {
    VALVE *from;
    VALVE *to;
    char from_label[2];
    char to_label[2];
    struct edge *next;
} EDGE;

typedef struct edge_list {
    EDGE *head;
}   EDGELIST;

extern VALVE valves[MAX_VALVES];
extern EDGELIST edges;
extern int distances[MAX_VALVES][MAX_VALVES], n_valves, used_valves;
extern int *interesting;

extern short (*paths)[PATH_WIDTH]; 
extern long pathindex; 

VALVE *find_valve(const char label[2]);
EDGE *add_edge(VALVE *from, VALVE *to);

void repair();
int valve_cmp(const void* a, const void* b);
void add_neighbours();
void set_indices();
void valve_info(void);
void compute_distances(void);
