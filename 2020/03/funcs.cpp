#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"

auto tree(const int64_t d_row, const int64_t d_col, const aoc::context& ctx) -> int64_t
{
    int64_t nrow = ctx.field.size();
    int64_t ncol = ctx.field[0].size();

    int64_t row = d_row, col = d_col, answer_1 = 0;

    while(row < nrow) 
    {
        if(ctx.field[row][col] == '#') answer_1++;

        col += d_col;
        col %= ncol;
        row += d_row;
    }

    return answer_1;
}