#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <string_view>

auto hash(std::string_view x) -> uint16_t;
