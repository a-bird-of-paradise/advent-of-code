#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>

std::string apply_rules(const std::string& base, const std::vector<aoc::rule_t>& rules);
long long score_1(const std::string& answer);