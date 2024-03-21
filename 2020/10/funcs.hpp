#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>

auto built_in_rating(const std::vector<int64_t>& adapters) -> int64_t;
auto count_paths(
    const std::map<int64_t, std::vector<int64_t>>& graph,
    const int64_t from,
    const int64_t last
) -> int64_t;
