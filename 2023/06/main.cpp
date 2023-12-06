#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    uint64_t answer_1 = 1, speed, time_left, distance_travelled;

    std::vector<std::pair<uint64_t,uint64_t>> data;
    for(std::size_t i = 0; i < Context.time.size(); i++)
        data.emplace_back(Context.time[i],Context.distance[i]);

    for(const auto& pair : data)
    {
        uint64_t temp = 0;
        for(uint64_t i = 0; i <= pair.first; i++) {
            speed = i;
            time_left = pair.first - i;
            distance_travelled = speed * time_left;
            if(distance_travelled > pair.second) temp++;
        }
        answer_1 *= temp;
    }

    // part 2 - trim spaces and run again

    std::cout << answer_1 << '\n';

    return 0;

}
