#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    enum class opcode { error, nop, jmp, acc, stop };
    enum class state { nominal, loop, fault, halt };

    struct instruction { aoc::opcode op; int32_t arg; };

    struct cpu { aoc::state state; int32_t accum; };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<instruction> instructions;
    };
}
