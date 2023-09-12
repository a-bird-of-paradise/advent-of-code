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

#pragma region // part one: first the easy bit ... 

    long answer_1 = 0; 

    for(auto & i : Context.lines)
        for(auto & j : i.second)
            if(j.size() == 2 || j.size() == 3 || j.size() == 4 || j.size() == 7)
                answer_1++;

    std::cout << answer_1 << std::endl;

#pragma endregion

#pragma region // validate every input has one of each unique length...

    long a2 = 0, a3 = 0, a4 = 0, a7 = 0;
    bool s2,s3,s4,s7;

    for(auto & i : Context.lines){
        s2 = false;
        s3 = false;
        s4 = false;
        s7 = false;
        for(auto & j : i.first) {
            switch(j.size()){
                case 2:
                    s2 = true;
                    break;
                case 3:
                    s3 = true;
                    break;
                case 4:
                    s4 = true;
                    break;
                case 7:
                    s7 = true;
                    break;
                default:
                    break;
            }
        }
        if(s2) a2++;
        if(s3) a3++;
        if(s4) a4++;
        if(s7) a7++;
    }

    std::cout << a2 << "|" << a3 <<"|" << a4 << "|" << a7 << "|" << Context.lines.size() << std::endl;

#pragma endregion

    return 0;
}