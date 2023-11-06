#pragma once
#include "context.hpp"
#include "parser.hpp"

void part_1(const aoc::context& Context);
void dirac_tick(int current_position, 
    std::vector<std::pair<uint64_t,uint64_t>>::iterator start,
    std::vector<std::pair<uint64_t,uint64_t>>::const_iterator end,
    int to_go = 21,
    uint64_t scenarios = 1) ;