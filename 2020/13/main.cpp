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

    // part 1

    int64_t min_bus = INT64_MAX, answer_1;

    for(const auto & bus : Context.line)
    {
        if(bus == 0) continue;
        int64_t temp = (Context.minutes / bus) * bus + bus;
        if(temp < min_bus) 
        {
            min_bus = temp;
            answer_1 = bus * mod(temp,Context.minutes);
        }
    }

    std::cout << answer_1 << '\n';

    // part 2 

    std::vector<int64_t> n, a;

    for(std::size_t i = 0; i < Context.line.size(); ++i)
    {
        if(Context.line[i] == 0) continue;
        n.push_back(Context.line[i]);
        a.push_back(Context.line[i]-i);
    }

    std::cout << chineseRemainder(n,a) << '\n';

    return 0;
}
