#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <cstdlib>

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    long min_pos, max_pos, least = INT32_MAX, least2 = INT32_MAX, accum, accum2, temp;

    min_pos = *std::min_element(Context.list.begin(), Context.list.end());
    max_pos = *std::max_element(Context.list.begin(), Context.list.end());

    for(long i = min_pos; i <= max_pos; i++)
    {
        accum = 0;
        accum2 = 0;
        
        for(auto & j : Context.list)
        {
            temp = abs(i - j);

            accum += temp;

            accum2 += temp * ( temp + 1) / 2;
        }

        if(accum < least) least = accum;
        if(accum2<least2) least2=accum2;

    }

    std::cout << least << std::endl;
    std::cout << least2<< std::endl;

    return 0;
}