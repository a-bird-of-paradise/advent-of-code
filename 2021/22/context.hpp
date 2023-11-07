#pragma once
#include "location.hh"
#include <vector>
#include <algorithm>

namespace aoc
{

    class cuboid
    {
    public:
        cuboid() : start_x(0), start_y(0), start_z(0), end_x(0), end_y(0), end_z(0) {}
        cuboid(long long x_0, long long x_1, long long y_0, long long y_1, long long z_0, long long z_1);
        bool intersects(const cuboid& other) const {
            bool answer = false;
            // x: test disjoint i.e. other is entirely to left (or right) of self
            if(end_x < other.start_x || start_x > other.end_x) { return answer; }
            if(end_y < other.start_y || start_y > other.end_y) { return answer; }
            if(end_z < other.start_z || start_z > other.end_z) { return answer; }
            answer = true; 
            return answer;
        }
        cuboid intersection(const cuboid& other) const {
            cuboid answer;
            if(!this->intersects(other)) return answer; 
            answer.start_x = std::max(this->start_x,other.start_x);
            answer.start_y = std::max(this->start_y,other.start_y);
            answer.start_z = std::max(this->start_z,other.start_z);

            answer.end_x = std::min(this->end_x, other.end_x);
            answer.end_y = std::min(this->end_y, other.end_y);
            answer.end_z = std::min(this->end_z, other.end_z);

            return answer;
        }
        long long volume() const {
            return (end_x - start_x + 1LL) * (end_y - start_y +1LL) * (end_z - start_z +1LL); 
        }
        long long start_x, start_y, start_z, end_x, end_y, end_z;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        long long start_x, start_y, start_z, end_x, end_y, end_z;
        bool is_on;
        std::vector< std::vector < std::vector<int>>> space;
        std::vector<std::pair<cuboid,bool>> cuboids;
    };
}
