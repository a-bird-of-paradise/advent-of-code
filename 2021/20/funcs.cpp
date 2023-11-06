#include "funcs.hpp"
#include "scanner.hpp"
#include <iostream>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

void print_field(const aoc::context& Context)
{
    for(const auto & i : Context.field) {
        for(const auto & j : i) {
            if(j==1) std::cout << '#'; else if(j==0) std::cout << '.'; else assert(false);
        }
        std::cout << std::endl;
    }
}

void iterate(aoc::context& Context)
{
    std::vector<std::vector<uint8_t>> new_field;

    std::size_t old_size = Context.field.size();
    std::size_t new_size = old_size + 2;

    std::vector<uint8_t> zero_row(new_size,0);

    // expand board

    new_field.resize(new_size);
    for(std::size_t i = 0; i < new_size; i++) new_field[i] = zero_row; // copy

    // mutate it

    uint8_t infinite;

    // following line needs very close reading of problem statement
    // also only works if algo[0]=0 or algo[0-9] = 1
    infinite = Context.current_iter % 2 == 0 ? 0 : Context.algo[0];

    std::size_t lookup, temp;

    for(std::size_t i = 0; i < new_size; i++) {
        for(std::size_t j = 0; j < new_size; j++) {
            lookup = 0;
            // top left - only if i,j >= 2
            if(i >= 2 && i < old_size + 2 && j >= 2 && j < old_size + 2) {
                temp = Context.field[i-2][j-2];
            } else {
                temp = infinite;
            }
            temp <<= 8;
            lookup += temp;
            
            // above
           if(i >= 2 && i < old_size + 2 && j >= 1 && j < old_size + 1)  {
                temp = Context.field[i-2][j-1];
            } else {
                temp = infinite;
            }
            temp <<= 7;
            lookup += temp;
            
            // top right
            if(i >= 2 && i < old_size + 2 && j >= 0 && j < old_size + 0)  {
                temp = Context.field[i-2][j  ];
            } else {
                temp = infinite;
            }
            temp <<= 6;
            lookup += temp;

            // left
            if(i >= 1 && i < old_size + 1 && j >= 2 && j < old_size + 2) {
                temp = Context.field[i-1][j-2];
            } else {
                temp = infinite;
            }
            temp <<= 5;
            lookup += temp;

            // self
            if(i >= 1 && i < old_size + 1 && j >= 1 && j < old_size + 1) {
                temp = Context.field[i-1][j-1];
            } else {
                temp = infinite;
            }
            temp <<= 4;
            lookup += temp;

            // right
            if(i >= 1 && i < old_size + 1 && j >= 0 && j < old_size + 0) {
                temp = Context.field[i-1][j  ];
            } else {
                temp = infinite;
            }
            temp <<= 3;
            lookup += temp;

            // bottom left
            if(i >= 0 && i < old_size + 0 && j >= 2 && j < old_size + 2) {
                temp = Context.field[i  ][j-2];
            } else {
                temp = infinite;
            }
            temp <<= 2;
            lookup += temp;

            // below
            if(i >= 0 && i < old_size + 0 && j >= 1 && j < old_size + 1) {
                temp = Context.field[i  ][j-1];
            } else {
                temp = infinite;
            }
            temp <<= 1;
            lookup += temp;

            // bottom right
            if(i >= 0 && i < old_size + 0 && j >= 0 && j < old_size + 0) {
                temp = Context.field[i  ][j  ];
            } else {
                temp = infinite;
            }
            temp <<= 0;
            lookup += temp;

            new_field[i][j] = Context.algo[lookup];
        }
    }
    Context.field = std::move(new_field);
    Context.current_iter++;
}
