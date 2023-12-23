#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>

auto build_graph(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>;
auto build_graph_2(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>;
auto get_index(int64_t x, int64_t y) -> int64_t;

