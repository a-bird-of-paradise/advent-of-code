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

    std::cout << tree(1,3,Context) << '\n';

    // part 2

    std::cout << 
        tree(1,1,Context)
        * tree(1,3,Context)
        * tree(1,5,Context) 
        * tree(1,7,Context)
        * tree(2,1,Context) 
        << '\n';

    return 0;
}