#pragma once
#include "context.hpp"
#include "parser.hpp"

auto get_zero_mask(const std::string& mask) -> uint64_t;
auto get_ones_mask(const std::string& mask) -> uint64_t;
auto get_floats(const std::string& mask) -> uint64_t;
auto get_addresses(uint64_t to_write, uint64_t float_mask) -> std::vector<uint64_t>;
