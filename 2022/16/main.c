#include "valves.h"
#include "lex.yy.h"
#include <x86intrin.h> // popcount 
#include <stdbool.h>

VALVE valves[MAX_VALVES] = {0};
EDGELIST edges = {0};
int used_valves = 0, n_valves = 0;
int distances[MAX_VALVES][MAX_VALVES];
int *interesting;
char *start = "AA";
long pathindex, nextpathindex;
short (*paths)[PATH_WIDTH];
bool need_new_index = false;    
int longest_for_hash[UINT16_MAX] = {0};

/*
      --BB
    /   |
AA      CC     FF--GG--HH
  \      |    /  
    \ --DD--EE
      \ 
        II---JJ
DD 2 
BB 5 (2+1) 3
JJ 9 (3+1) 4
HH 17 (7+1) 8
EE 21 (3+1) 4
CC 24 (2+1) 3
*/

unsigned path_hash(const long i,const int n)
{
    unsigned answer = 0;
    for(int j = 0; j <= n; j++)
        if(paths[i][j] != 0)
            answer |= (1<<paths[i][j]);
    return answer;
}

int path_flow(const long i, const int tmax)
{
    int time = 0, dtime;
    int flow = 0, dflow;

    unsigned hash_so_far;
    int max_so_far; 

    for(int j = 1; j < PATH_WIDTH; j++)
    {
        if(!paths[i][j]) break;

        hash_so_far = path_hash(i,j);
        max_so_far = longest_for_hash[hash_so_far];

        dtime = distances[valves[interesting[paths[i][j-1]]].index]
            [valves[interesting[paths[i][j]]].index] + 1;

        dflow = valves[interesting[paths[i][j]]].flow_rate;

        time += dtime;
        if(time>tmax) break;
        flow += dflow * (tmax - time);

        if(flow > max_so_far) longest_for_hash[hash_so_far] = flow; 

    }
    return flow;
}

void pathfinder(int index, int time, unsigned long visited, long pathindex)
{
    int nexttime;
    if(time >= 30) return;
    paths[pathindex][_mm_popcnt_u32(visited)]=index;

    visited |= (1<<index);

    for(int i = 1; i <= used_valves; i++)
    {

        if(i == index) continue;

        if(visited & (1<<i)) continue;

        if(index == 0){ // start state

            nexttime = time + distances[find_valve(start)->index][valves[interesting[i]].index];

            if(nexttime >= 30) continue;

            if(need_new_index) { nextpathindex++; need_new_index = false;   }

            memcpy(paths[nextpathindex],paths[pathindex],16 * sizeof(short));
            pathfinder(i,nexttime,visited,nextpathindex);

            need_new_index = true;

        } else {

            nexttime = time + distances[valves[interesting[i]].index][valves[interesting[index]].index];

            if(nexttime >= 30) continue;

            if(need_new_index) { nextpathindex++; need_new_index = false;   }

            memcpy(paths[nextpathindex],paths[pathindex],16 * sizeof(short));
            pathfinder(i,nexttime,visited, nextpathindex);

            need_new_index = true;
        }
    }
}

int main(int argc, char **argv)
{
    yylex();

    if(used_valves > 16) exit(1);

    qsort(valves,MAX_VALVES,sizeof(VALVE),valve_cmp);
    repair();
    add_neighbours();
    set_indices();
    compute_distances();

    interesting = malloc((1+used_valves) * sizeof(int));

    int i = 0;
    VALVE *this_valve = valves;
    while(i < used_valves) {
        if(this_valve->flow_rate>0) {
            interesting[i+1]=this_valve->index;
            i++;
        }
        this_valve++;
    }
    interesting[0]=0; // used as a start state 

    pathindex = 0; 
    nextpathindex=1;

    paths = calloc(N_PATHS, sizeof(*paths));

    pathfinder(0,0,0,pathindex);

    int this_flow, max_flow = INT16_MIN, maxi;

    for(long i = 0; i < nextpathindex; i++)
    {
        this_flow = path_flow(i,30);
        if(this_flow > max_flow) {
            maxi = i;
            max_flow = this_flow;
        }
    }
    valve_info();

    printf("%10d%10d\n",maxi,max_flow);

    // reset hash table for 26

    for(unsigned i = 0; i < UINT16_MAX; i++) longest_for_hash[i]=0;
    for(long i = 0; i < nextpathindex; i++) path_flow(i,26);
    
    int answer = 0, temp = 0;

    for(unsigned i = 0; i < UINT16_MAX; i++) {
        for(unsigned j = i; j < UINT16_MAX; j++) {
            if((i & j) == 0) {
                temp = longest_for_hash[i]+longest_for_hash[j];
                answer = temp > answer ? temp : answer;
            }
        }
    }

    printf("%10d\n",answer);

    free(interesting);
    free(paths);

    return 0;
}