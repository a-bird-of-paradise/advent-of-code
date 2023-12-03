#pragma once
#include "location.hh"
#include <vector>
#include <set>

namespace aoc
{

    class symbol
    {
    public:
        symbol() : value(' '), col(0), row(0) {}
        symbol(char val, int row_no, int column_no) : value(val), col(column_no), row(row_no) {}
        auto operator<=>(const symbol&) const = default;
        char value;
        int col;
        int row;
    };

    class number
    {
    public:
        number() : value(0), row(0), col_start(0), col_end(0) {}
        number(long num, int row_num, int col_start_num, int col_end_num) 
            : value(num), row(row_num), col_start(col_start_num), col_end(col_end_num) {}
        auto operator<=>(const number&) const = default;
        long value;
        int row;
        int col_start;
        int col_end;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<symbol> symbols;
        std::vector<number> numbers;
    };
}
