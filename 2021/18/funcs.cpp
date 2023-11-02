#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}
