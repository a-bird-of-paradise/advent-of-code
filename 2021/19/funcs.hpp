#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <iostream>

bool test_distinct_sumsq(const std::vector<aoc::point>& locs);
bool test_distinct_manhattan(const std::vector<aoc::point>& locs);
bool test_distinct_fingerprint(const std::vector<aoc::point>& locs);
long long sq_distance(const aoc::point& p, const aoc::point& q);
long long manhattan_distance(const aoc::point& p, const aoc::point& q);
std::ostream& operator<<(std::ostream& os, const aoc::point& p);
aoc::point operator+(const aoc::point& lhs, const aoc::point& rhs);
aoc::point operator-(const aoc::point& lhs, const aoc::point& rhs);

aoc::point rotate(aoc::point p, std::size_t rot_index);
long long magnitude_sq(const aoc::point& p);
