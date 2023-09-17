#pragma once
#include "context.hpp"
#include "parser.hpp"

long pathfinder(const aoc::graph& Graph);

aoc::graph make_graph(const std::vector<std::vector<int>>& board);

std::vector<std::vector<int>> embiggen(const std::vector<std::vector<int>>& tile);