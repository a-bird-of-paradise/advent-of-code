#pragma once
#include "context.hpp"
#include "parser.hpp"

std::vector<long long> 
    apply_map(
        const std::vector<long long>& source, 
        const std::vector<aoc::triple>& map
    );

long long apply_map_single ( long long source, const std::vector<aoc::triple>& map);