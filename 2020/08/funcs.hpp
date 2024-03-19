#pragma once
#include "context.hpp"
#include "parser.hpp"

auto sim(const std::vector<aoc::instruction>& instructions) -> aoc::cpu;
