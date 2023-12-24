#pragma once
#include "context.hpp"
#include "parser.hpp"

auto operator<<(std::ostream& stream, const aoc::line_t& line) -> std::ostream&;
auto intersect_2d(const aoc::line_t& left, const aoc::line_t& right) -> std::pair<aoc::point_t,bool>;
auto inside_2d(const std::pair<aoc::point_t,bool>& crossing, int64_t lower, int64_t upper) -> bool;
