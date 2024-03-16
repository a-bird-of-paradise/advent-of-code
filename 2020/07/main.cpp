#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <queue>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // part 1

    std::set<aoc::bag_colour_t> visited;
    std::queue<aoc::bag_colour_t> to_visit;

    for(const auto & bag : Context.parents[std::make_pair("shiny","gold")]) to_visit.push(bag);

    while(!to_visit.empty())
    {
        visited.insert(to_visit.front());
        for(const auto & bag : Context.parents[to_visit.front()])   to_visit.push(bag);
        to_visit.pop();
    }

    std::cout << visited.size() << '\n';

    // part 2

    std::cout << num_children(Context,std::make_pair("shiny","gold")) << '\n';

    return 0;
}
