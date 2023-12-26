#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    for(const auto & n : Context.numbers)
    {
        if(Context.numbers.contains(2020 - n)) {
            std::cout << n * (2020 - n) << '\n';
            break;
        }
    }

    std::set<int64_t>::const_iterator first, second;

    first = std::begin(Context.numbers);
    std::advance(first,1);
    bool done = false;

    while(first != std::end(Context.numbers) && !done)
    {
        second = std::begin(Context.numbers);

        while(second != first)
        {
            if(Context.numbers.contains(2020 - *first - *second))
            {
                done = true;
                std::cout << (*first) * (*second) * (2020 - *first - *second) << '\n';
                break;
            }
            std::advance(second,1);
        }
        std::advance(first,1);
    }

    return 0;
}