#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}
