#pragma once
#include "context.hpp"
#include "parser.hpp"

auto adjacent(const aoc::symbol& sym, const aoc::number& num) -> bool;

auto operator<< (std::ostream& stream, const aoc::symbol& sym) -> std::ostream&;
auto operator<< (std::ostream& stream, const aoc::number& num) -> std::ostream&;