#pragma once
#include "context.hpp"
#include "parser.hpp"

auto string_to_hand_type(std::string const& s) -> aoc::hand_type;
auto compare_cards(char l, char r) -> bool;
auto compare_cards_2(char l, char r) -> bool;

auto string_to_hand_type_2(std::string const& s) -> aoc::hand_type;