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

    std::stack<int> states;
    states.push(0);

    int answer = 0;

    for(const auto & s : Context.strings)
        if(match(s,states,Context.rules))
            answer++;

    std::cout << answer << '\n';

    // part 2

    answer = 0;

    aoc::rule eight { aoc::rule_type::nonterminal, '!' };
    eight.rules.push_back ({42});
    eight.rules.push_back ({ 42, 8 });

    aoc::rule eleven { aoc::rule_type::nonterminal, '!' };
    eleven.rules.push_back ({ 42, 31 });
    eleven.rules.push_back ({ 42, 11, 31 });

    Context.rules[8] = eight;
    Context.rules[11] = eleven;

   for(const auto & s : Context.strings)
        if(match(s,states,Context.rules))
            answer++;

    std::cout << answer << '\n';
    return 0;
}