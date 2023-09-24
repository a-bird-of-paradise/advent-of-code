#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.set_debug_level(0);

    Context.terminator_counter.push(std::make_pair(1,1));

    Parser.parse();
    
    std::cout << "Version sum: " << Context.version_sum << std::endl;

    return 0;

}