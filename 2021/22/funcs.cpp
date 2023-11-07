#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

std::ostream& operator<< (std::ostream& stream, const aoc::cuboid& c)
{
    stream  << "[("
            << c.start_x
            << ','
            << c.start_y
            << ','
            << c.start_z 
            << "),("
            << c.end_x
            << ','
            << c.end_y
            << ','
            << c.end_z 
            << ")]";
    return stream;
}

aoc::cuboid::cuboid(long long x_0, long long x_1, long long y_0, long long y_1, long long z_0, long long z_1)
            :   start_x(x_0),start_y(y_0),start_z(z_0),
                end_x(x_1),end_y(y_1),end_z(z_1) {
  //                  assert(start_x <= end_x);
    //                assert(start_y <= end_y);
      //              assert(start_z <= end_z);
                }