#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <set>
#include <algorithm>
#include <stack>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    aoc::point p{0,0};

    std::set<aoc::point> visited;
    visited.insert(p);

    for(std::size_t i = 0; i < Context.dir.size(); ++i)
    {
        for(std::size_t j = 0; j < Context.length[i]; ++j)
        {
            switch(Context.dir[i])
            {
                case 'U':   p.y--;  break;
                case 'D':   p.y++;  break;
                case 'L':   p.x--;  break;
                case 'R':   p.x++;  break;
                default:    assert(false);
            }
            visited.insert(p);
        }
    }

    int64_t max_x = std::ranges::max_element(visited,[](const auto& l, const auto& r){return l.x<r.x;})->x;
    int64_t max_y = std::ranges::max_element(visited,[](const auto& l, const auto& r){return l.y<r.y;})->y;
    int64_t min_x = std::ranges::min_element(visited,[](const auto& l, const auto& r){return l.x<r.x;})->x;
    int64_t min_y = std::ranges::min_element(visited,[](const auto& l, const auto& r){return l.y<r.y;})->y;

    int64_t nrow = max_y - min_y + 1, ncol = max_x - min_x + 1;

    std::vector<std::vector<char>> field;
    field.resize(nrow);
    for(auto&& x : field) x.resize(ncol);
    for(auto&& r : field) for(auto&& c : r) c = '.';

    for(const auto & p : visited) field[p.y - min_y][p.x - min_x]= '#';

    std::stack<aoc::point> to_flood;

    for(int64_t i = 0; i < field.size(); i++)
        if(field[i][0] == '.')
            to_flood.push(aoc::point{0,i});

    for(int64_t i = 0; i < field[0].size(); i++)
        if(field[0][i] == '.')
            to_flood.push(aoc::point{i,0});

    for(int64_t i = 0; i < field.size(); i++)
        if(field[i].back() == '.')
            to_flood.push(aoc::point(field[i].size()-1,i));

    for(int64_t i = 0; i < field[0].size(); i++)
        if(field.back()[i] == '.')
            to_flood.push(aoc::point(i,field.size()-1));

    while(!to_flood.empty())
    {
        p = to_flood.top();
        to_flood.pop();

        field[p.y][p.x] = 'F';

        if(p.y > 0 && field[p.y-1][p.x] == '.') to_flood.push({p.x,p.y-1});
        if(p.x > 0 && field[p.y][p.x-1] == '.') to_flood.push({p.x-1,p.y});
        if(p.y < field.size()-1 && field[p.y+1][p.x] == '.') to_flood.push({p.x,p.y+1});
        if(p.x < field[0].size()-1 && field[p.y][p.x+1] == '.') to_flood.push({p.x+1,p.y});
    }

    int64_t count_F = 0, count_hash = 0, count_dot = 0;

    for(const auto & row : field) {
        for(const auto & col : row) {
            switch(col) {
                case 'F':   count_F++;      break;
                case '#':   count_hash++;   break;
                case '.':   count_dot++;    break;
                default:    assert(false);
            }
        }
    }

    std::cout << count_dot + count_hash << '\n';

    // part_2 

    std::vector<aoc::point> points;

    p = {0,0};
    points.push_back(p);

    int64_t disp = 0;

    for(const auto& colour : Context.colour)
    {
        int64_t displacement = std::stoll(colour.substr(0,5),0,16);
        disp += displacement;

        switch(colour.back())
        {
            case    '0':    p.x += displacement;    break;  //  R
            case    '1':    p.y += displacement;    break;  //  D
            case    '2':    p.x -= displacement;    break;  //  L
            case    '3':    p.y -= displacement;    break;  //  U
            default:    assert(false);
        }
        points.push_back(p);
    }
    points.push_back({0,0});

    int64_t answer_2 = 0;

    for(auto i = 1; i < points.size(); i++)
    {
        answer_2 += (points[i].y * points[i-1].x);
        answer_2 -= (points[i].x * points[i-1].y);
    }

    std::cout << answer_2 / 2 + disp / 2 + 1 << '\n';
    // not quite sure why the +1 is needed!

    return 0;
}
