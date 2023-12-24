#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <boost/rational.hpp>

extern bool noisy;

auto  operator<<(std::ostream& stream, const aoc::line_t& line) -> std::ostream&
{
    stream << "[(" << line[0] << ',' << line[1] << ',' << line[2] << "), (";
    stream << line[3] << ',' << line[4] << ',' << line[5] << ")]";
    return stream;
}

auto intersect_2d(const aoc::line_t& left, const aoc::line_t& right) -> std::pair<aoc::point_t,bool>
{
    std::pair<aoc::point_t,bool> answer;

    bool neg_left = left[3] < 0, neg_right = right[3] < 0;
    bool zero_left = left[3] == 0, zero_right = right[3] == 0;

    // infinite gradient

    if(zero_left && zero_right){
        answer.second = false;
        return answer; 
    }
    aoc::rational_t a, b, c, d;

    if(zero_left) {
        answer.second = true;
        answer.first.first = left[0];
        answer.first.second=1;
        if(!neg_right) b = aoc::rational_t(right[4],right[3]); else b = aoc::rational_t(-right[4],-right[3]);
        c = right[1] - b * right[0];
        answer.first.second = b * left[0] + c;
        return answer;
    }

    if(zero_right) {
        answer.second = true;
        answer.first.first = right[0];
        answer.first.second=1;
        if(!neg_right) b = aoc::rational_t(left[4],left[3]); else b = aoc::rational_t(-left[4],-left[3]);
        c = left[1] - b * left[0];
        answer.first.second = b * right[0] + c;
        return answer;
    }

    if(!neg_left) a = aoc::rational_t(left[4],left[3]); else a = aoc::rational_t(-left[4],-left[3]);
    if(!neg_right) b = aoc::rational_t(right[4],right[3]); else b = aoc::rational_t(-right[4],-right[3]);

    c = aoc::rational_t (left[1]-a*left[0]);
    d = aoc::rational_t (right[1]-b*right[0]);

   // parallel 

    if(a == b) {
        answer.second = c == d;
    } else {
        answer.second = true;
        answer.first.first = aoc::rational_t(d-c);
        answer.first.first /= (a-b);
        answer.first.second = aoc::rational_t(d-c);
        answer.first.second *= a; 
        answer.first.second /= (a-b);
        answer.first.second += c;
    }
    return answer;
}

auto inside_2d(const std::pair<aoc::point_t,bool>& crossing, int64_t lower, int64_t upper) -> bool
{
    if(crossing.second == false) return false;

    return (crossing.first.first >= lower 
        && crossing.first.second >= lower 
        && crossing.first.first <= upper 
        && crossing.first.second <= upper);
}
