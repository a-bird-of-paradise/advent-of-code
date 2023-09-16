#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    Parser.parse();

    std::size_t max_x, max_y;

    max_x = std::max_element(
        Context.points.begin(),
        Context.points.end(),
        [](const aoc::point_t& lhs, const aoc::point_t& rhs)
            {   return lhs.first < rhs.first;   }
        )->first;
    max_y = std::max_element(
        Context.points.begin(),
        Context.points.end(),
        [](const aoc::point_t& lhs, const aoc::point_t& rhs)
            {   return lhs.second < rhs.second; }
        )->second;

    std::cout << "(" << max_x << "," << max_y << ")" << std::endl;

    std::vector<std::vector<char>> field(max_y+1, std::vector<char>(max_x+1, '.'));

    for(auto & point : Context.points) field[point.second][point.first] = '#';

    for(auto & fold : Context.folds){
        std::cout << score(field) << std::endl;
        apply_fold(field,fold);
    }

    print_field(field);

    return 0;
}