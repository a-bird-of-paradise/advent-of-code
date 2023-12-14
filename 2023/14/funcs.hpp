#pragma once
#include "context.hpp"
#include "parser.hpp"

auto tick_n(std::vector<std::string>& field) -> uint32_t;
auto tick_e(std::vector<std::string>& field) -> uint32_t;
auto tick_w(std::vector<std::string>& field) -> uint32_t;
auto tick_s(std::vector<std::string>& field) -> uint32_t;
auto cycle(std::vector<std::string>& field) -> void;
auto print(const std::vector<std::string>& field) -> void;
auto load(const std::vector<std::string>& field) -> uint32_t;