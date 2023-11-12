#pragma once
#include "context.hpp"
#include "parser.hpp"

void print(const std::vector<std::vector<char>>& x);

std::vector<std::vector<char>> step(const std::vector<std::vector<char>>& prev);