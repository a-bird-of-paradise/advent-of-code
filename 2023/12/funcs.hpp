#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <set>

auto generate_candidates(const std::vector<char>& current,
                        std::size_t level,
                        std::set<std::vector<char>>& candidates, 
                        const std::vector<int64_t>& report) -> void;

auto is_valid_candidate(const std::vector<char>& candidate, 
                        const std::vector<int64_t>& report) -> bool;


auto is_definitely_invalid(
    const std::vector<char>& candidate, 
    const std::vector<int64_t>& report) -> bool;


auto get_report(const std::vector<char>& candidate) -> const std::vector<int64_t>;

auto recurse(const std::string& candidate, const std::vector<int64_t>& unused) -> int64_t;
