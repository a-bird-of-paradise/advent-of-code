#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto sim(const std::vector<aoc::instruction>& instructions) -> aoc::cpu
{
    aoc::cpu answer;
    answer.state = aoc::state::nominal;
    answer.accum = 0;

    int32_t instruction_pointer = 0; 

    std::vector<int16_t> seen(instructions.size(),0);

    while(answer.state == aoc::state::nominal)
    {
        seen[instruction_pointer]++;

        switch(instructions[instruction_pointer].op) 
        {
            case aoc::opcode::jmp:
                instruction_pointer += instructions[instruction_pointer].arg;
                break;
            case aoc::opcode::nop:
                instruction_pointer++;
                break;
            case aoc::opcode::acc:
                answer.accum += instructions[instruction_pointer].arg;
                instruction_pointer++;
                break;
            case aoc::opcode::error:
                answer.state = aoc::state::fault;
                break;
            case aoc::opcode::stop:
                answer.state = aoc::state::halt;
                break;
        }

        if(answer.state != aoc::state::halt) {

            if(instruction_pointer < 0 || instruction_pointer > instructions.size())
                answer.state = aoc::state::fault;

            if(seen[instruction_pointer] != 0)
                answer.state = aoc::state::loop;
        }
    }

    return answer;
}