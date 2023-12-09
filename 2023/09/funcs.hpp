#pragma once
#include "context.hpp"
#include "parser.hpp"

auto complete_history(const std::vector<int64_t>& history) -> std::pair<int64_t,int64_t>;