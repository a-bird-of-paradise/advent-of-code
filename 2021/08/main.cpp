#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    if(Parser.parse() != 0) abort();

    unsigned counts[10] = {0}, answer = 0, accum;

    for(auto & line : Context.lines)
    {
        unsigned pattern[10] = {0};

        pattern[1] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [](unsigned i) { return std::popcount(i) == 2; }
        );

        pattern[4] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [](unsigned i) { return std::popcount(i) == 4; }
        );

        pattern[7] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [](unsigned i) { return std::popcount(i) == 3; }
        );

        pattern[8] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [](unsigned i) { return std::popcount(i) == 7; }
        );

        pattern[3] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 5 && (i & pattern[7]) == pattern[7]; }
        );

        pattern[9] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 6 && (i & pattern[4]) == pattern[4]; }
        );

        pattern[5] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 5 
                && (i | pattern[9]) == pattern[9]
                && i != pattern[3]; }
        );

        pattern[2] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 5 
                && i != pattern[5]
                && i != pattern[3]; }
        );

        pattern[6] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 6 
                && (i & pattern[5]) == pattern[5]
                && i != pattern[9]; }
        );

        pattern[0] = *std::find_if(
            line.first.begin(),
            line.first.end(),
            [&](unsigned i) { return std::popcount(i) == 6 
                && i != pattern[6]
                && i != pattern[9]; }
        ); 

        accum = 1;
        
        for(auto & j : line.second)
        {
            for(unsigned k = 0; k < 10; k++)
            {
                if(j == pattern[k]) {
                    counts[k]++;
                    accum *= 10;
                    accum += k;
                    break;
                }

            }
        }

        accum -= 10000;
        answer += accum;

    }

    std::cout << counts[1] + counts[4] + counts[7] + counts[8] << std::endl;
    std::cout << answer << std::endl;

    return 0;
}