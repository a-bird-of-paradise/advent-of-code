#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <iostream>

unsigned long item_cost(const aoc::state& item);
aoc::state from_letter(char letter);
unsigned long solve(const aoc::board& game);

std::ostream& operator<< (std::ostream& stream, const aoc::board& b);
std::ostream& operator<< (std::ostream& stream, const aoc::state& s);

bool is_amber(const aoc::state& x);
bool is_bronze(const aoc::state& x);
bool is_copper(const aoc::state& x);
bool is_desert(const aoc::state& x);
bool is_vacant(const aoc::state& x);
bool is_unoccupiable(const aoc::state& x);
bool is_item(const aoc::state& x);
bool is_not_item(const aoc::state& x);
