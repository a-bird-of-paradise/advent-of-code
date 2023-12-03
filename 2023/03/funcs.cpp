#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto adjacent(const aoc::symbol& sym, const aoc::number& num) -> bool
{
    bool answer = true;
    int row_dist = sym.row - num.row;

    if(row_dist > 1 || row_dist < -1) answer = false;
    if(sym.col < num.col_start - 1 || sym.col > num.col_end + 1) answer = false;

    return answer;
}

auto operator<< (std::ostream& stream, const aoc::symbol& sym) -> std::ostream&
{
    stream << sym.value << " (" << sym.row << ',' << sym.col << ')';
    return stream;
}

auto operator<< (std::ostream& stream, const aoc::number& num) -> std::ostream&
{
    stream << num.value << " (" << num.row << ',' << num.col_start << ',' << num.col_end << ')';
    return stream;
}