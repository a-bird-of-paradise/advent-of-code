#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <map>
#include <algorithm>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::cout << path_length(
        Context.field,
        aoc::beam{aoc::direction::right,0,0}
    ) << '\n';

    // part 2

    int32_t answer_2 = INT32_MIN, temp;
    int32_t nrow = Context.field.size();
    int32_t ncol = Context.field[0].size();

    for(int32_t i = 0; i < nrow; i++)
        answer_2 = std::max(path_length(Context.field, aoc::beam{aoc::direction::right,i,0}),answer_2);
    for(int32_t i = 0; i < nrow; i++)
        answer_2 = std::max(path_length(Context.field, aoc::beam{aoc::direction::left,i,ncol-1}),answer_2);
    for(int32_t i = 0; i < ncol; i++)
        answer_2 = std::max(path_length(Context.field, aoc::beam{aoc::direction::down,0,i}),answer_2);
    for(int32_t i = 0; i < ncol; i++)
        answer_2 = std::max(path_length(Context.field, aoc::beam{aoc::direction::up,nrow-1,i}),answer_2);

    std::cout << answer_2 << '\n';

    return 0;
}
