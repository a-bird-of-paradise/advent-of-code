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
 
    std::cout << combat(Context.p1,Context.p2) << '\n';
    std::cout << recursive_combat(Context.p1,Context.p2).first << '\n';

    return 0;
}