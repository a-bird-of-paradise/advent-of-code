#pragma once
#include "context.hpp"

long score(const std::vector<std::vector<char>>& field);

void apply_fold(std::vector<std::vector<char>>& field, const aoc::fold_t& fold);

void print_field(const std::vector<std::vector<char>> & field);
