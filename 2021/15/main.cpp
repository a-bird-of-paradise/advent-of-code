#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    std::cout << pathfinder(make_graph(Context.board)) << std::endl;
    std::cout << pathfinder(make_graph(embiggen(Context.board))) << std::endl;

    return 0;

}