#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>

int main (int argc, char **argv)
{

    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    long long part_1 = 0, part_2 = 0;

    while(Context.current_iter < 50) {

        iterate(Context);

        if(Context.current_iter == 2){
            for(const auto & i : Context.field){ 
                for(const auto & j : i) {
                    assert(j == 0 || j == 1);
                    part_1 += j;
                }
            }
        }

        if(Context.current_iter == 50) {
            for(const auto & i : Context.field){ 
                for(const auto & j : i) {
                    assert(j == 0 || j == 1);
                    part_2 += j;
                }
            }
        }
    }

    std::cout << part_1 << std::endl;
    std::cout << part_2 << std::endl;

    return 0;

}