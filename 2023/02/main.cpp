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

    uint32_t answer_1 = 0, answer_2 = 0;
    uint32_t least_red, least_green, least_blue;
    bool can_win;

    for(const auto& games : Context.file) {

        can_win = true;
        least_red = least_blue = least_green = 0;
        
        for(const auto& plays : games.second) {
            for(const auto& turn : plays) {
                switch(turn.first){
                    case aoc::colour::RED:
                        if (turn.second > 12) can_win = false;
                        if (turn.second > least_red) least_red = turn.second;
                        break;
                    case aoc::colour::GREEN:
                        if (turn.second > 13) can_win = false;
                        if (turn.second > least_green) least_green = turn.second;
                        break;
                    case aoc::colour::BLUE:
                        if (turn.second > 14) can_win = false;
                        if (turn.second > least_blue) least_blue = turn.second;
                        break;
                }
            }
        }
        if(can_win) answer_1 += games.first;
        answer_2 += least_red * least_green * least_blue;
    }

    std::cout << answer_1 << '\n' << answer_2 << '\n';

    return 0;

}
