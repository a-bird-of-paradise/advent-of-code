#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>
#include <algorithm>

int aoc::error_state;
unsigned long long aoc::answer_2;

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);
    
    aoc::error_state = 0;
    aoc::answer_2 = 0;

    Parser.set_debug_level(0);
    Parser.parse();

    std::cout << Context.answer_1 << std::endl;

    std::sort(
        Context.line_totals.begin(),
        Context.line_totals.end()
    );

    std::cout << Context.line_totals[
        Context.line_totals.size()/2
    ] << std::endl;

    return 0;
}