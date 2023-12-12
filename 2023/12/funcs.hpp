#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <set>

auto generate_candidates(const std::vector<char>& current,
                        std::size_t level,
                        std::set<std::vector<char>>& candidates) -> void;

auto is_valid_candidate(const std::vector<char>& candidate, 
                        const std::vector<int64_t>& report) -> bool;
