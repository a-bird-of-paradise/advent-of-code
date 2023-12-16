#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <map>

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto tick_n(std::vector<std::string>& field) -> uint32_t
{
    uint32_t counter = 0;
    for(std::size_t i = 1; i < field.size(); ++i)
    {
        for(std::size_t j = 0; j < field[i].size(); ++j) 
        {
            if(field[i][j] == 'O' && field[i-1][j] == '.') 
            {
                field[i][j] = '.';
                field[i-1][j] = 'O';
                counter++;
            }
        }
    }
    return counter;
}

auto tick_e(std::vector<std::string>& field) -> uint32_t
{
    uint32_t counter = 0;
    for(std::size_t i = 0; i < field.size(); ++i)
    {
        for(std::size_t j = 0; j < field[i].size()-1; ++j) 
        {
            if(field[i][j] == 'O' && field[i][j+1] == '.') 
            {
                field[i][j] = '.';
                field[i][j+1] = 'O';
                counter++;
            }
        }
    }
    return counter;
}

auto tick_s(std::vector<std::string>& field) -> uint32_t
{
    uint32_t counter = 0;
    for(std::size_t i = 0; i < field.size()-1; ++i)
    {
        for(std::size_t j = 0; j < field[i].size(); ++j) 
        {
            if(field[i][j] == 'O' && field[i+1][j] == '.') 
            {
                field[i][j] = '.';
                field[i+1][j] = 'O';
                counter++;
            }
        }
    }
    return counter;
}

auto tick_w(std::vector<std::string>& field) -> uint32_t
{
    uint32_t counter = 0;
    for(std::size_t i = 0; i < field.size(); ++i)
    {
        for(std::size_t j = 1; j < field[i].size(); ++j) 
        {
            if(field[i][j] == 'O' && field[i][j-1] == '.') 
            {
                field[i][j] = '.';
                field[i][j-1] = 'O';
                counter++;
            }
        }
    }
    return counter;
}

auto cycle(std::vector<std::string>& field) -> void
{
    while(tick_n(field)!=0);
    while(tick_w(field)!=0);
    while(tick_s(field)!=0);
    while(tick_e(field)!=0);
}

auto print(const std::vector<std::string>& field) -> void
{
    for(const auto& row : field) {
        for(const auto& col : row) {
            std::cout << col;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

auto load(const std::vector<std::string>& field) -> uint32_t
{
    uint32_t answer = 0;

    for(std::size_t i = 0; i < field.size(); ++i) {
        for(std::size_t j = 0; j < field[i].size(); ++j) {
            if(field[i][j] == 'O') {
                answer += field.size() - i;
            }
        }
    }

    return answer;
}