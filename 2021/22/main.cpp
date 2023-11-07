#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse(); // bison actions do part 1 out of laziness

    std::vector<std::pair<aoc::cuboid,bool>> actions, new_actions;

    for(auto const& input : Context.cuboids) {
        // replicate part 1?
        if(false)
            if(input.first.start_x < -50 || input.first.start_y < -50 || input.first.start_z < -50
            || input.first.end_x > 50 || input.first.end_y > 50 || input.first.end_z > 50)
                continue;

        // create a new action for every interesection with existing actions

        for(auto const& action : actions) {
            if(action.first.intersects(input.first)) {
                new_actions.push_back(std::make_pair(
                    action.first.intersection(input.first),
                    !action.second)
                );
            }
        }

        if(input.second) new_actions.push_back(input); // only happens if input is an on cube

        for(auto const & new_action : new_actions) actions.push_back(new_action);

        new_actions.clear();
    }

    unsigned long long answer = 0;
    for(auto const& action : actions) {
        if(action.second) {
            answer += action.first.volume();
        } else {
            answer -= action.first.volume();
        }
    }

    std::cout << answer << std::endl;

    return 0;

}
