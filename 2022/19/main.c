#include "structs.h"
#include "lex.yy.h"
#include <stdbool.h>

BLUEPRINT blueprints[N_BLUEPRINTS] = {0};

int pmax(int a, int b) { if(a>b) return a; else return b; }

void print_blueprint(const int i) {
    printf("%4d%4d%4d%4d%4d%4d%4d\n",
        blueprints[i].id,
        blueprints[i].ore_robot_ore_cost,
        blueprints[i].clay_robot_ore_cost,
        blueprints[i].obsidian_robot_ore_cost,
        blueprints[i].obsidian_robot_clay_cost,
        blueprints[i].geode_robot_clay_cost,
        blueprints[i].geode_robot_obsidian_cost);
}
void print_inventory(CONTEXT ctx){
    printf("%4d%4d%4d%4d%4d\n",ctx.clay,ctx.ore,ctx.obsidian,ctx.geode,ctx.current_minute);
}

long max_geodes(CONTEXT inventory) 
{
    //print_inventory(inventory);
    // stop condition
    if(inventory.current_minute == inventory.max_minute) {
        return inventory.geode + inventory.geode_collecting_robots;
    }

    static long max_geodes_seen;
    if(inventory.current_minute == 1) max_geodes_seen = INT32_MIN;

    // prune hopeless paths
    int dt = inventory.max_minute - inventory.current_minute;
    long best_case_max = 
        inventory.geode // what we currently have
        + dt * (dt+1)/2 // how many more robots we could possibly produce
        + dt * inventory.geode_collecting_robots; // current bot production;

    if(best_case_max < max_geodes_seen) return inventory.geode;

    long answer = INT32_MIN;
    long temp = answer;

    // pruning
    int orebot_max = 
        pmax(blueprints[inventory.blueprint_id].clay_robot_ore_cost,
            pmax(blueprints[inventory.blueprint_id].ore_robot_ore_cost,
                blueprints[inventory.blueprint_id].obsidian_robot_ore_cost));
    int claybot_max = 
        pmax(blueprints[inventory.blueprint_id].obsidian_robot_clay_cost,
            blueprints[inventory.blueprint_id].geode_robot_clay_cost);
    bool skip = false;

    // make copies of inventory
    CONTEXT children[5];
    for(int i = 0; i < 5; i++){
        memcpy(&children[i], &inventory, sizeof(inventory));
        children[i].current_minute++;
    }

    // decide whether to do nothing, or build one of the four robots, this minute
    // build geobot if poss
    if(children[0].clay >= blueprints[children[0].blueprint_id].geode_robot_clay_cost
        && children[0].obsidian >= blueprints[children[0].blueprint_id].geode_robot_obsidian_cost)
        {
            // consume
            children[0].clay -= blueprints[children[0].blueprint_id].geode_robot_clay_cost;
            children[0].obsidian -= blueprints[children[0].blueprint_id].geode_robot_obsidian_cost;
            // harvest
            children[0].ore += children[0].ore_collecting_robots;
            children[0].clay += children[0].clay_collecting_robots;
            children[0].obsidian += children[0].obsidian_collecting_robots;
            children[0].geode += children[0].geode_collecting_robots;
            // produce
            children[0].geode_collecting_robots++;
            //recurse
            temp = max_geodes(children[0]);
            skip = true;
        }
    answer = answer < temp ? temp : answer;

    // build obsbot if poss
    if(children[1].clay >= blueprints[children[1].blueprint_id].obsidian_robot_clay_cost
        && children[1].ore >= blueprints[children[1].blueprint_id].obsidian_robot_ore_cost
        && !skip)
        {
            // consume
            children[1].clay -= blueprints[children[1].blueprint_id].obsidian_robot_clay_cost;
            children[1].ore -= blueprints[children[1].blueprint_id].obsidian_robot_ore_cost;
            // harvest
            children[1].ore += children[1].ore_collecting_robots;
            children[1].clay += children[1].clay_collecting_robots;
            children[1].obsidian += children[1].obsidian_collecting_robots;
            children[1].geode += children[1].geode_collecting_robots;
            // produce
            children[1].obsidian_collecting_robots++;
            //recurse
            temp = max_geodes(children[1]);
        }
    answer = temp > answer ? temp : answer;
    
    // build claybot if poss
    // and worthwhile
    if(children[2].ore >= blueprints[children[2].blueprint_id].clay_robot_ore_cost 
        && children[2].clay_collecting_robots < claybot_max
        && !skip)
        {
            // consume
            children[2].ore -= blueprints[children[2].blueprint_id].clay_robot_ore_cost;
            // harvest
            children[2].ore += children[2].ore_collecting_robots;
            children[2].clay += children[2].clay_collecting_robots;
            children[2].obsidian += children[2].obsidian_collecting_robots;
            children[2].geode += children[2].geode_collecting_robots;
            // produce
            children[2].clay_collecting_robots++;
            //recurse
            temp = max_geodes(children[2]);
        }
    answer = temp > answer ? temp : answer;

    // build orebot if poss
    // but only if we aren't producing too many i.e. cannot consume any more
    if(children[3].ore >= blueprints[children[3].blueprint_id].ore_robot_ore_cost &&
        children[3].ore_collecting_robots < orebot_max
        && !skip)
        {
            // consume
            children[3].ore -= blueprints[children[3].blueprint_id].ore_robot_ore_cost;
            // harvest
            children[3].ore += children[3].ore_collecting_robots;
            children[3].clay += children[3].clay_collecting_robots;
            children[3].obsidian += children[3].obsidian_collecting_robots;
            children[3].geode += children[3].geode_collecting_robots;
            // produce
            children[3].ore_collecting_robots++;
            //recurse
            temp = max_geodes(children[3]);
        }
    answer = temp > answer ? temp : answer;

    // nothing
    // harvest
    if(!skip) {
        children[4].ore += children[4].ore_collecting_robots;
        children[4].clay += children[4].clay_collecting_robots;
        children[4].obsidian += children[4].obsidian_collecting_robots;
        children[4].geode += children[4].geode_collecting_robots;
        temp = max_geodes(children[4]); 
    }
    answer = temp > answer ? temp : answer;

    max_geodes_seen = answer > max_geodes_seen ? answer : max_geodes_seen;

    return answer;
}

int main(int argc, char **argv)
{
    yylex(); // fill blueprints

    for(int i = 0; i < N_BLUEPRINTS; i++)
    {
        if(!blueprints[i].id) break;
        if(false) print_blueprint(i);
    }

    CONTEXT initial = {0};
    initial.current_minute=1;
    initial.ore_collecting_robots=1;
    initial.max_minute = 24;

    long answer = 0;

    if(true)
    for(int i = 0; i < N_BLUEPRINTS; i++)
    {
        if(!blueprints[i].id) break;
        initial.blueprint_id = i;
        long temp = max_geodes(initial);
        printf("%4d%8ld\n",i,temp);
        answer += (i+1)*temp;
    }

    printf("%8ld\n",answer);

    initial.max_minute = 32;
    answer = 1;
    if(true)
    for(int i = 0; i < 3; i++)
    {
        if(!blueprints[i].id) break;
        initial.blueprint_id = i;
        long temp = max_geodes(initial);
        printf("%4d%8ld\n",i,temp);
        answer *= temp;
    }

    printf("%8ld\n",answer);

    return 0;
}