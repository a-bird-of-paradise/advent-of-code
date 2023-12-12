#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <ranges>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    int64_t answer_1 = 0;

    for(std::size_t i = 0; i < Context.field.size(); ++i)
    {
        std::cout << "::" << i << '\n';

        std::set<std::vector<char>> candidates;

        for(const auto & c : Context.field[i]) std::cout << c;
        std::cout << '\n';

        generate_candidates(Context.field[i],0,candidates);

        for(const auto& candidate : candidates){
            if(is_valid_candidate(candidate,Context.contiguous[i])) {
                answer_1++;
            }
        }
    }

    std::cout << answer_1 << '\n';

    return 0;
}
