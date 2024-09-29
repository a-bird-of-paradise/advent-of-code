#pragma once
#include "context.hpp"
#include "parser.hpp"

auto operator<<(std::ostream& stream, const aoc::tile& tile) -> std::ostream&;
auto operator<<(std::ostream& stream, const aoc::field_t& tile) -> std::ostream&;

auto rotate_identity(aoc::tile to_rotate) -> aoc::tile;
auto rotate_left(aoc::tile to_rotate) -> aoc::tile;
auto rotate_right(aoc::tile to_rotate) -> aoc::tile;
auto rotate_180(aoc::tile to_rotate) -> aoc::tile;
auto flip_horizontal(aoc::tile to_rotate) -> aoc::tile;
auto flip_vertical(aoc::tile to_rotate) -> aoc::tile;
auto flip_diag(aoc::tile to_rotate) -> aoc::tile;
auto flip_antidiag(aoc::tile to_rotate) -> aoc::tile;

auto apply_rotation(int id, aoc::tile to_rotate) -> aoc::tile;

auto could_x_be_left_of_y(const aoc::tile& x, const aoc::tile& y) -> bool;
auto could_x_be_right_of_y(const aoc::tile& x, const aoc::tile& y) -> bool;
auto could_x_be_up_of_y(const aoc::tile& x, const aoc::tile& y) -> bool;
auto could_x_be_down_of_y(const aoc::tile& x, const aoc::tile& y) -> bool;

auto is_empty_state(const aoc::tile_state& x) -> bool;

auto backtrack_root(const std::map<int, aoc::tile>& tiles) -> aoc::field_t;
auto backtrack_reject(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> bool;
auto backtrack_accept(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> bool;
auto backtrack_first(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> std::pair<bool,aoc::field_t>;
auto backtrack_next(const std::map<int,aoc::tile>& tiles, const aoc::field_t& field) -> std::pair<bool,aoc::field_t>;
auto backtrack(const std::map<int,aoc::tile>& tiles, aoc::field_t& answer) -> void; 

auto render(const std::map<int, aoc::tile>& tiles, const aoc::field_t& field) -> aoc::tile;

auto get_monster(void) -> std::vector<std::string>;
auto monster_could_be_at(const aoc::tile& tile, int row, int col) -> bool ;
