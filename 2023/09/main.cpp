#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    int64_t answer_1 = 0, answer_2 = 0;

    for(const auto& h : Context.histories)
    {
        auto answers = complete_history(h);
        answer_1 += answers.first;
        answer_2 += answers.second;
    }

    std::cout << answer_1 << '\n' << answer_2 << '\n';

    return 0;
}
