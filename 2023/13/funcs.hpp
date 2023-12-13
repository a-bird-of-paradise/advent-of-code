#pragma once
#include "context.hpp"
#include "parser.hpp"

auto print(const aoc::note& note) -> void;
auto reflect_h(const aoc::note& note, std::size_t index) -> aoc::note;
auto reflect_v(const aoc::note& note, std::size_t index) -> aoc::note;
auto get_reflect_indices(int size, int index) -> std::vector<std::size_t>;

auto is_reflected_h(const aoc::note& note, std::size_t index) -> bool;
auto is_reflected_v(const aoc::note& note, std::size_t index) -> bool;
