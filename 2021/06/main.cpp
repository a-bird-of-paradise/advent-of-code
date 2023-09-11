#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"

long evolve(long initial, long ticks)
{
    long state[9], answer = 0, temp;

    std::fill_n(state,9,0);

    state[initial]++;

    while(ticks--)
    {
        temp = state[0];
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] = state[4];
        state[4] = state[5];
        state[5] = state[6];
        state[6] = state[7] + temp;
        state[7] = state[8];
        state[8] = temp;
    }

    for(std::size_t j = 0; j < 9; j++) answer += state[j];

    return answer;
}

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    long state[9], n, temp, answer;
    const long ticks_1 = 18, ticks_2 = 80, ticks_3 = 256;

    answer = 0;
    for(auto& i : Context.list) answer += evolve(i,ticks_1);
    std::cout << answer << std::endl;

    answer = 0;
    for(auto& i : Context.list) answer += evolve(i,ticks_2);
    std::cout << answer << std::endl;

    answer = 0;
    for(auto& i : Context.list) answer += evolve(i,ticks_3);
    std::cout << answer << std::endl;

    return 0;
}