#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}


void print(const std::vector<std::vector<char>>& x)
{
    for(const auto& i : x) {
        for(const auto& j : i) {
            std::cout << j;
        }
        std::cout << '\n';
    }
    return;
}

std::vector<std::vector<char>> step(const std::vector<std::vector<char>>& prev)
{
    std::vector<std::vector<char>> next = prev, mid = prev;

    // move east herd

    for(std::size_t i = 0; i < mid.size(); i++) {
        for(std::size_t j = 0; j < mid[i].size()-1; j++) { // special for last col
            if(prev[i][j] == '>' && prev[i][j+1] == '.') {
                mid[i][j] = '.';
                mid[i][j+1] = '>';
            }
        }
        if(prev[i].back() == '>' && prev[i][0] == '.') {
            mid[i].back() = '.';
            mid[i][0] = '>';
        }
    }

    next = mid;

    // move south herd
    for(std::size_t col = 0; col < next[0].size(); col++) {
        for(std::size_t row = 0; row < next.size() - 1; row++) {
            if(mid[row][col] == 'v' && mid[row+1][col] == '.') {
                next[row][col] = '.';
                next[row+1][col] = 'v';
            }
        }
        if(mid.back()[col] == 'v' && mid[0][col] == '.') {
            next.back()[col] = '.';
            next[0][col] = 'v';
        }
    }

    return next;
}