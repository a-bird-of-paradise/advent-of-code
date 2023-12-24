#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>

auto build_graph(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>;
auto build_graph_2(const std::vector<std::string>& field) -> std::map<int64_t, std::vector<int64_t>>;
auto get_index(int64_t x, int64_t y) -> int64_t;

auto dfs(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    const int64_t goal,
    const int64_t current,
    std::vector<int64_t>& current_path,
    std::vector<std::vector<int64_t>>& paths
    ) -> void;

auto compact(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    int64_t start,
    int64_t end
    ) -> std::map<int64_t, std::vector<std::pair<int64_t,int64_t>>>;
