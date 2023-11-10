#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <set>

std::vector<long long> precdents(long long w, long long A, long long B, long long d, long long z);

struct cmp
{
    bool operator()(const std::vector<long long>& a,const std::vector<long long>& b) const;
};

void dfs(const aoc::context& ctx,
        std::vector<long long>& current_number,
        std::set<std::vector<long long>, cmp>& valid_numbers,
        std::size_t depth,
        long long z_n);
