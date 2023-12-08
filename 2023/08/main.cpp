#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::sort(Context.lines.begin(), Context.lines.end(),
    [](auto& l, auto& r)
    {
        aoc::hand_type left = string_to_hand_type(l.first);
        aoc::hand_type right = string_to_hand_type(r.first);

        if(left == right)
        {
            if(l.first[0] != r.first[0]) return compare_cards(l.first[0],r.first[0]);
            if(l.first[1] != r.first[1]) return compare_cards(l.first[1],r.first[1]);
            if(l.first[2] != r.first[2]) return compare_cards(l.first[2],r.first[2]);
            if(l.first[3] != r.first[3]) return compare_cards(l.first[3],r.first[3]);
            return compare_cards(l.first[4],r.first[4]);
        }
        return left < right;
    });

    uint64_t answer_1 = 0;
    for(std::size_t i = 0; i < Context.lines.size(); i++) answer_1 += (i+1) * Context.lines[i].second;
    std::cout << answer_1 << '\n';

    // part 2

    std::sort(Context.lines.begin(), Context.lines.end(),
    [](auto& l, auto& r)
    {
        aoc::hand_type left = string_to_hand_type_2(l.first);
        aoc::hand_type right = string_to_hand_type_2(r.first);

        if(left == right)
        {
            if(l.first[0] != r.first[0]) return compare_cards_2(l.first[0],r.first[0]);
            if(l.first[1] != r.first[1]) return compare_cards_2(l.first[1],r.first[1]);
            if(l.first[2] != r.first[2]) return compare_cards_2(l.first[2],r.first[2]);
            if(l.first[3] != r.first[3]) return compare_cards_2(l.first[3],r.first[3]);
            return compare_cards_2(l.first[4],r.first[4]);
        }
        return left < right;
    });

    uint64_t answer_2 = 0;
    for(std::size_t i = 0; i < Context.lines.size(); i++) answer_2 += (i+1) * Context.lines[i].second;
    std::cout << answer_2 << '\n';

    return 0;

}
