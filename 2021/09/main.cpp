#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include <iomanip>
#include <algorithm>

void fill(const aoc::point_t& point, 
    std::vector<aoc::line_t>& field,
    std::vector<aoc::point_t>& basin)
{
    if(field[point.first][point.second] == 9) return;

    basin.push_back(point);
    field[point.first][point.second] = 9;

    if(point.first != 0 && field[point.first - 1][point.second] < 9)
        fill(std::make_pair(point.first-1,point.second),field,basin);

    if(point.first != field.size()-1 && field[point.first + 1][point.second] < 9)
        fill(std::make_pair(point.first+1,point.second),field,basin);

    if(point.second != 0 && field[point.first][point.second - 1] < 9)
        fill(std::make_pair(point.first,point.second-1),field,basin);

    if(point.second != field[point.first].size()-1 && field[point.first][point.second + 1] < 9)
        fill(std::make_pair(point.first,point.second+1),field,basin);
}

int main (int argc, char **argv)
{

    aoc::scanner Scanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(Scanner,Context);

    if(Parser.parse() != 0) abort();

    aoc::int_t up, down, left, right, current;

    long long answer_1 = 0, answer_2 = 1;

    std::vector<aoc::point_t> low_points;

    for(std::size_t i = 0; i < Context.lines.size(); i++) {
        for(std::size_t j = 0; j < Context.lines[i].size(); j++) {

            up    = i == 0                          ? aoc::int_t_max : Context.lines[i-1][j];
            down  = i == Context.lines.size()-1     ? aoc::int_t_max : Context.lines[i+1][j];
            left  = j == 0                          ? aoc::int_t_max : Context.lines[i][j-1];
            right = j == Context.lines[i].size()-1  ? aoc::int_t_max : Context.lines[i][j+1];

            current = Context.lines[i][j];

            if(current < up && current < down && current < left && current < right)
            {
                low_points.push_back(std::make_pair(i,j));

                current += 1;
                answer_1 += current;
            }
        }
    }

    std::cout << answer_1 << std::endl;

    // part 2: flood fill basins

    std::vector<std::vector<aoc::point_t>> basins;

    for(auto & i : low_points)
    {
        std::vector<aoc::point_t> basin;
        fill(i,Context.lines,basin);
        basins.push_back(std::move(basin));
    }

    std::sort( // descending order... 
        basins.begin(), 
        basins.end(),
        [](const std::vector<aoc::point_t>& lhs, const std::vector<aoc::point_t>& rhs){
            return rhs.size() < lhs.size();
        }
        );

    answer_2 = basins[0].size() * basins[1].size() * basins[2].size();

    std::cout << answer_2 << std::endl;

    return 0;
}