#include "funcs.hpp"

long score(const std::vector<std::vector<char>>& field)
{
    long answer = 0;
    for(auto & i : field)
        for(auto & j : i)
            if(j == '#')
                answer++;

    return answer;
}

void apply_fold(std::vector<std::vector<char>>& field, const aoc::fold_t& fold)
{
    char fold_axis = fold.first;
    std::size_t fold_loc = fold.second;

    std::size_t max_x = field[0].size()-1;
    std::size_t max_y = field.size()-1;

    // field[col][row]

    if(fold_axis == 'y')
    {
        for(int i = fold_loc; i >= 0; i--)
        {
            for(int j = 0; j <= max_x; j++) {
                if(field[fold_loc + i][j] == '#') {
                    field[fold_loc-i][j] = '#';
                    field[fold_loc+i][j] = '.';
                }
            }
        }
        field.resize(fold_loc);
    } else { 
        for(int i = fold_loc; i >= 0; i--)
        {
            for(int j = 0; j <= max_y; j++) {
                if(fold_loc + i <= max_x && field[j][fold_loc + i] == '#') {
                    field[j][fold_loc-i] = '#';
                    field[j][fold_loc+i] = '.';
                }
            }
        }
        for(auto & i : field) i.resize(fold_loc);
    }
}

void print_field(const std::vector<std::vector<char>> & field)
{
    for(auto & i : field) {
        for(auto & j : i ){
            if(j == '.') std::cout << " ";
            else std::cout << j;
        }
        std::cout << std::endl;
    }
}
