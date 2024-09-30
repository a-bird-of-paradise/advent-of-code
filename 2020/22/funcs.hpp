#pragma once
#include "context.hpp"
#include "parser.hpp"

auto combat(std::deque<int> p1, std::deque<int> p2, bool noisy = false) -> uint64_t;
auto recursive_combat(std::deque<int> p1, std::deque<int> p2, bool noisy = false) -> std::pair<uint64_t,int>;