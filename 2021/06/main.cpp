#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    std::vector<long> today = Context.list, tomorrow;
    long n_new;

    for(int i = 0; i < 256; i++)
    {
        tomorrow.clear();
        n_new = 0;

        for(auto & i : today)
        {
            if(i == 0) {
                n_new++;
                tomorrow.push_back(6);
            }
            else {
                tomorrow.push_back(i-1);
            }
        }

        while(n_new--) tomorrow.push_back(8);

        today = tomorrow;
        //for(auto & i : today) std::cout << std::setw(2) << i;
        //std::cout << std::endl;
    }

    std::cout << today.size() << std::endl;

    return 0;
}