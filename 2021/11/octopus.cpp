#include "octopus.hpp"

const int aoc::octopus::distance(const aoc::octopus& other)
{
    int dx = this->location.first - other.location.first;
    int dy = this->location.second - other.location.second;

    dx = dx < 0 ? -dx : dx;
    dy = dy < 0 ? -dy : dy;

    return dx + dy;
}