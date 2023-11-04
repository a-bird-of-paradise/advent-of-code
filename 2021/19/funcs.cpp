#include "funcs.hpp"
#include "scanner.hpp"
#include <algorithm>
#include <cmath>

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

bool test_distinct_sumsq(const std::vector<aoc::point>& locs) {
    // test - can we use square distance as a fingerprint? 
    bool answer;
    std::vector<long long> distances;

    for(auto i = locs.begin(); i != locs.end(); i++) 
        for(auto j = std::next(i); j != locs.end(); j++) 
            distances.push_back(sq_distance(*i,*j));
        
    std::sort(distances.begin(),distances.end());
    auto i = std::adjacent_find(distances.begin(),distances.end());
    answer = i == distances.end() ? true : false;
    return answer;
}

bool test_distinct_manhattan(const std::vector<aoc::point>& locs) {
    // test - can we use manhattan distance as a fingerprint?
    bool answer;
    std::vector<long long> distances;

    for(auto i = locs.begin(); i != locs.end(); i++) 
        for(auto j = std::next(i); j != locs.end(); j++) 
            distances.push_back(manhattan_distance(*i,*j));
        
    std::sort(distances.begin(),distances.end());
    auto i = std::adjacent_find(distances.begin(),distances.end());
    answer = i == distances.end() ? true : false;
    return answer;
}

bool test_distinct_fingerprint(const std::vector<aoc::point>& locs) {
    // test - can we use manhattan distance as a fingerprint?
    bool answer;
    std::vector<aoc::fingerprint> distances;

    for(auto i = locs.begin(); i != locs.end(); i++) 
        for(auto j = std::next(i); j != locs.end(); j++) 
            distances.emplace_back(*i,*j);
        
    std::sort(distances.begin(),distances.end());
    auto i = std::adjacent_find(distances.begin(),distances.end());
    answer = i == distances.end() ? true : false;
    return answer;
}

long long sq_distance(const aoc::point& p, const aoc::point& q){
    return (q.x-p.x)*(q.x-p.x) + (q.y-p.y)*(q.y-p.y) + (q.z-p.z)*(q.z-p.z);
}

long long manhattan_distance(const aoc::point& p, const aoc::point& q) {
    return abs(p.x-q.x) + abs(p.y-q.y) + abs(p.z-q.z);
}

aoc::fingerprint::fingerprint(const point& x, const point& y) 
        : sumsq(sq_distance(x,y)), manhattan(manhattan_distance(x,y)) {} ;


std::ostream& operator<<(std::ostream& os, const aoc::point& p) {
    os << p.x << "," << p.y << "," << p.z;
    return os;
}

aoc::point operator+(const aoc::point& lhs, const aoc::point& rhs){
    return aoc::point(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
}
aoc::point operator-(const aoc::point& lhs, const aoc::point& rhs){
    return aoc::point(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
}

// helper funcs not in header
void rot_x(aoc::point& p) { int temp = p.y; p.y = -p.z; p.z = temp; }
void rot_y(aoc::point& p) { int temp = p.z; p.z = -p.x; p.x = temp; }
void rot_z(aoc::point& p) { int temp = p.x; p.x = -p.y; p.y = temp; }

// relying on compiler to optimise this
aoc::point rotate(aoc::point p, std::size_t rot_index) {
    // apply an element of S4 to p. 
    switch(rot_index) {
    case 1: // identity
        break;
    case 2: // rotate around y
        rot_y(p);
        break;
    case 3:
        rot_y(p); rot_y(p);
        break;
    case 4:
        rot_y(p); rot_y(p); rot_y(p);
        break;
    case 5: // rotate 1x around x then over y
        rot_x(p);
        break;
    case 6:
        rot_x(p); rot_y(p);
        break;
    case 7:
        rot_x(p); rot_y(p); rot_y(p);
        break;
    case 8:
        rot_x(p); rot_y(p); rot_y(p); rot_y(p);
        break;
    case 9: // rotate 3x around x then over y (2x around x is the same as 2y2z later)
        rot_x(p); rot_x(p); rot_x(p); ;
        break;
    case 10:
        rot_x(p); rot_x(p); rot_x(p); rot_y(p);
        break;
    case 11:
        rot_x(p); rot_x(p); rot_x(p); rot_y(p); rot_y(p);
        break;
    case 12:
        rot_x(p); rot_x(p); rot_x(p); rot_y(p); rot_y(p); rot_y(p);
        break;
    case 13: // rorate around z then y
        rot_z(p);
        break;
    case 14:
        rot_z(p); rot_y(p);
        break;
    case 15:
        rot_z(p); rot_y(p); rot_y(p);
        break;
    case 16:
        rot_z(p); rot_y(p); rot_y(p); rot_y(p);
        break;
    case 17: // rorate around z 2x then y
        rot_z(p); rot_z(p);
        break;
    case 18:
        rot_z(p); rot_z(p); rot_y(p);
        break;
    case 19:
        rot_z(p); rot_z(p); rot_y(p); rot_y(p);
        break;
    case 20:
        rot_z(p); rot_z(p); rot_y(p); rot_y(p); rot_y(p);
        break;
    case 21: // rorate around z 3x then y
        rot_z(p); rot_z(p); rot_z(p);
        break;
    case 22:
        rot_z(p); rot_z(p); rot_z(p); rot_y(p);
        break;
    case 23:
        rot_z(p); rot_z(p); rot_z(p); rot_y(p); rot_y(p);
        break;
    case 24:
        rot_z(p); rot_z(p); rot_z(p); rot_y(p); rot_y(p); rot_y(p);
        break;
    default:
        assert(false); // S4 has 24 elements, fault if incorrect call
        break;
    }

    return p;
}

long long magnitude_sq(const aoc::point& p){
    return p.x*p.x + p.y*p.y + p.z*p.z;
}