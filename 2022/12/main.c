#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXROWS 100

typedef struct node {
    bool visited;
    unsigned distance;
    struct node_list_item *first_neighbour;
}   NODE;

typedef struct node_list_item {
    struct node_list_item *next;
    NODE    *this_node;
}   LISTITEM;

typedef struct node_list {
    LISTITEM *head;
}   NODELIST;

NODE **map;
char **textfile;

NODE *start, *end;
NODELIST to_visit, starts;

int ncols, rowcounter=0, nstarts = 0;
unsigned *startdistances;

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

void fill()
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
    while(!end->visited)
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
        if(smallest->this_node->distance == UINT16_MAX) break; // inaccessible
        visit(smallest->this_node);
    }
}

void reset_nodes()
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

int cmpfunc(const void *a, const void *b)
{
   const unsigned *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int main(int argc, char *argv[])
{
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