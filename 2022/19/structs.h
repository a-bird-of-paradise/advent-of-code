#pragma once
#define N_BLUEPRINTS 30

typedef struct blueprint_t {
    long id;
    long ore_robot_ore_cost;
    long clay_robot_ore_cost;
    long obsidian_robot_ore_cost;
    long obsidian_robot_clay_cost;
    long geode_robot_clay_cost;
    long geode_robot_obsidian_cost;
} BLUEPRINT;

typedef struct inventory_context_t {
    long blueprint_id;
    long current_minute;
    long ore_collecting_robots;
    long ore;
    long clay_collecting_robots;
    long clay;
    long obsidian_collecting_robots;
    long obsidian;
    long geode_collecting_robots;
    long geode;
    long max_minute;
} CONTEXT;

extern BLUEPRINT blueprints[N_BLUEPRINTS];
