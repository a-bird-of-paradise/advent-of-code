#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <queue>

auto print(const std::map<std::string,aoc::module>& modules) -> void;

auto process_queue(
    std::queue<aoc::pulse>& input, 
    std::queue<aoc::pulse>& output, 
    std::map<std::string,aoc::module>& modules,
    std::map<bool,int64_t>& counter) -> bool;
