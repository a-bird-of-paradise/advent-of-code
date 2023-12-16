#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto hash(std::string_view x) -> uint16_t
{
    uint16_t answer = 0;

    for(const auto& c : x)
    {
        answer += c;
        answer *= 17;
        answer %= 256;
    }

    return answer;
}