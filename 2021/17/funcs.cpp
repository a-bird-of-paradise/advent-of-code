#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

bool finishes_in_range(const aoc::context& Context, int start_dx, int start_dy)
{
    bool answer = false;

    int x = 0, y = 0;

    while(!answer && y >= Context.ymin) {

        if(Context.xmin <= x
            && Context.xmax >= x
            && Context.ymin <= y
            && Context.ymax >= y) answer = true;

        x += start_dx; 
        y += start_dy;

        start_dy--;
        if(start_dx > 0) start_dx--; else if(start_dx<0) start_dx++;
    }

    return answer;

}