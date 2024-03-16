#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <numeric>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    std::vector<int32_t> seen(26), all(26);
    int32_t answer_1 = 0, answer_2 = 0;

    for(const auto& group : Context.groups)
    {
        for(auto&& i : seen) i = 0;
        for(auto&& i : all) i = 0;

        for(const auto& str : group)
        {
            for(const auto& chr : str) 
            {
                seen[chr - 'a'] = 1;
                all[chr - 'a'] += 1;
            }
        }

        answer_1 += std::accumulate(std::begin(seen),std::end(seen),0);
        answer_2 += std::accumulate(
            std::begin(all),
            std::end(all),
            0,
            [&](int32_t a, int32_t b) {return b == group.size() ? a + 1 : a;}
        );
    }

    std::cout << answer_1 << '\n' << answer_2 << '\n';

    return 0;
}
