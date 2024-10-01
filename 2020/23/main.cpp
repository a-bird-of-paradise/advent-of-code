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

    std::vector<int32_t> cups = Context.input;

    int32_t current_cup = cups.front();

    for(int32_t i = 0; i < 100; ++i) {
        int32_t next = tick(cups,current_cup);
        current_cup = next;
    }

    std::vector<int32_t>::const_iterator it = std::find(
        std::begin(cups),
        std::end(cups),
        1
    );

    std::cout << '\n';

    ++it; 
    if(it == std::end(cups)) it = std::begin(cups);

    for(int32_t i = 0; i < 9-1; ++i)
    {
        std::cout << *it;
        ++it;
        if(it == std::end(cups)) it = std::begin(cups);
    }

    std::cout << '\n';

    // part 2

    std::vector<int32_t> part_2;
    for(int32_t i = 0; i < 1'000'000; ++i)
    {
        if(i < Context.input.size()) part_2.push_back(Context.input[i]);
        else part_2.push_back(i+1);
    }

    current_cup = part_2.front();

    for(int32_t i = 0; i < 10'000'000; ++i) {
        int32_t next = tick(part_2,current_cup);
        current_cup = next;
    }

    int64_t answer; 

    it = std::find(std::begin(part_2),std::end(part_2),1);
    it = std::next(it);
    if(it == std::end(part_2)) it = std::begin(part_2);
    std::cout << *it << ' ';
    answer = *it;
    it = std::next(it);
    if(it == std::end(part_2)) it = std::begin(part_2);
    std::cout << *it << ' ';
    answer *= *it;

    std::cout << answer << '\n';

    return 0;
}