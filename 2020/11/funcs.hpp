#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>

auto print(const std::vector<std::string>& board) -> void;
auto tick(const std::vector<std::string>& board) -> std::vector<std::string>;
auto equals(const std::vector<std::string>& left, const std::vector<std::string>& right) -> bool;

// so for part 2 ... 

auto get_index(
    const std::size_t x,
    const std::size_t y, 
    const std::size_t n_x) -> std::size_t;

auto index_to_x(
    const std::size_t index,
    const std::size_t n_y) -> std::size_t;

auto index_to_y(
    const std::size_t index, 
    const std::size_t n_y) -> std::size_t;

auto build_neighbours(
    const std::vector<std::string>& board
) -> std::map<std::size_t, std::vector<std::size_t>>;

auto build_neighbours_2(
    const std::vector<std::string>& board
) -> std::map<std::size_t, std::vector<std::size_t>>;

auto tick_state(
    const std::string& state,
    const std::map<std::size_t, std::vector<std::size_t>>& neighbours,
    const std::size_t n_y,
    const uint16_t n_occupied = 4
) -> std::string;

