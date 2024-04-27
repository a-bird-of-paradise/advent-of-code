#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    //aoc::scanner TokenScanner(std::cin, std::cout);
    //aoc::context Context;
    //aoc::parser Parser(TokenScanner,Context);

    //Parser();

    std::cout << do_sim({0,3,6}) << '\n';
    std::cout << do_sim({1,3,2}) << '\n';
    std::cout << do_sim({2,1,3}) << '\n';
    std::cout << do_sim({1,2,3}) << '\n';
    std::cout << do_sim({2,3,1}) << '\n';
    std::cout << do_sim({3,2,1}) << '\n';
    std::cout << do_sim({3,1,2}) << '\n';
    std::cout << do_sim({19,0,5,1,10,13}) << '\n';
    std::cout << do_sim({19,0,5,1,10,13},30'000'000) << '\n';

    return 0;
}
