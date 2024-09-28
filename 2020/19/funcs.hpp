#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <stack>

auto match(
    std::string_view to_match, 
    std::stack<int> states,
    const std::map<int, aoc::rule>& rules) -> bool;