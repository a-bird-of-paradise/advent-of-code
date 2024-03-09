#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    int32_t max_seen = INT32_MIN;

    std::set<int> seen;

    for(const auto & ticket : Context.tickets)
    {
        int upper = 127;
        int lower = 0;
        int width = upper - lower + 1;

        int row, column;

        for(std::size_t i = 0; i < 7; ++i) {
            width /= 2;
            if(ticket[i] == 'F') {
                upper = lower + width - 1;
            } else { // 'B'
                lower = lower + width;
            }
        }

        row = lower; // == upper 

        upper = 7;
        lower = 0;
        width = upper - lower + 1;

        for(std::size_t i = 7; i < 10; ++i) {
            width /= 2;
            if(ticket[i] == 'L') {
                upper = lower + width - 1;
            } else { // 'R'
                lower = lower + width;
            }
        }

        column = lower;

        int32_t id = 8*row + column;

        seen.insert(id);

        max_seen = max_seen < id ? id : max_seen;

    }

    std::cout << max_seen << '\n';

    for(const auto & id : seen)
        if(!seen.contains(id+1) && seen.upper_bound(id) != seen.end()) {
            std::cout << id + 1 << '\n';
        }

    return 0;
}
