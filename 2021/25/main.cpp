#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    auto prev = Context.lines;
    auto next = step(prev);
    unsigned long long counter = 1;

    while(next != prev) {
        //print(next);
        //std::cout << counter << '\n';
        counter++;
        prev = next;
        next = step(prev);
    }

    //print(next);

    std::cout << counter << std::endl;

    return 0;

}
