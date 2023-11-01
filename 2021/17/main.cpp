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

    std::cout << (Context.ymin) * (1+Context.ymin) / 2 << std::endl;

    // neat. part 2.

    int i;

    Context.max_dy = -Context.ymin - 1;
    Context.max_dx = Context.xmax;
    Context.min_dy = Context.ymin;
    i = 0;

    while(i * (i+1) / 2 < Context.xmin) i++;
    Context.min_dx = i;

    int answer = 0;

    for(int i = Context.min_dx; i <= Context.max_dx; i++) 
        for(int j = Context.min_dy; j <= Context.max_dy; j++)
            if(finishes_in_range(Context,i,j))
                answer++;

    std::cout << answer << std::endl;

    return 0;

}