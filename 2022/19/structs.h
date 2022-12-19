#pragma once
#define N_BLUEPRINTS 30

typedef struct blueprint_t {
    int id;
    int ore_robot_ore_cost;
    int clay_robot_ore_cost;
    int obsidian_robot_ore_cost;
    int obsidian_robot_clay_cost;
    int geode_robot_ore_cost;
    int geode_robot_obsidian_cost;
} BLUEPRINT;

typedef struct inventory_context_t {
    int blueprint_id;
    int current_minute;
    int ore_collecting_robots;
    int ore;
    int clay_collecting_robots;
    int clay;
    int obsidian_collecting_robots;
    int obsidian;
    int geode_collecting_robots;
    int geode;
    int max_minute;
} CONTEXT;

extern BLUEPRINT blueprints[N_BLUEPRINTS];
