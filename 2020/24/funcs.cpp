#include "funcs.hpp"
#include "scanner.hpp"

std::ostream& aoc::operator<<(std::ostream& os, const aoc::move& dt) {
    os << '(' << dt.e << ',' << dt.nw << ')';
    return os;
}
std::ostream& operator<<(std::ostream& os, const aoc::move& dt) {
    os << '(' << dt.e << ',' << dt.nw << ')';
    return os;
}