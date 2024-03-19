#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    // part 1 

    std::cout << sim(Context.instructions).accum << '\n';

    // part 2 

    aoc::cpu answer; 
    std::vector<aoc::instruction> altered;

    for(std::size_t i = 0; i < Context.instructions.size(); ++i) {
        altered = Context.instructions;
        switch(Context.instructions[i].op)
        {
            case aoc::opcode::nop:
                altered[i].op = aoc::opcode::jmp;
                answer = sim(altered);
                if(answer.state == aoc::state::halt) std::cout << answer.accum << '\n';
                break;
            case aoc::opcode::jmp:
                altered[i].op = aoc::opcode::nop;
                answer = sim(altered);
                if(answer.state == aoc::state::halt) std::cout << answer.accum << '\n';
                break;
            default:
                break;
        }
    }

    return 0;
}
