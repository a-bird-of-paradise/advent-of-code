#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <map>

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto print(const aoc::note& note) -> void
{
    for(const auto& r : note) {
        for(const auto& c : r) {
            std::cout << c;
        }
        std::cout << '\n';
    }
}

auto get_reflect_indices(int size, int index) -> std::vector<std::size_t>
{
    std::vector<std::size_t> answer;
    std::size_t first, last, inc;

    if(size < 0 || index < 0 || index > size) return answer;

    if(index < size/2){
        first = 0;
        last = index;
        inc = 1;
    } else {
        first = size-1;
        last = index+1;
        inc = -1;
    }

    for(std::size_t i = first; i != last + inc; i += inc) answer.push_back(i);
    for(std::size_t i = last; i != first - inc; i -= inc) answer.push_back(i);

    return answer;
}

auto reflect_h(const aoc::note& note, std::size_t index) -> aoc::note
{
    std::vector<std::size_t> indices = get_reflect_indices(note.size(),index);
    aoc::note answer;
    for(const auto& i : indices) answer.push_back(note[i]);
    return answer;
}

auto is_reflected_h(const aoc::note& note, std::size_t index) -> bool
{
    std::size_t first, last, inc;
    std::size_t first_b, last_b, inc_b;
    std::size_t size = note.size();

    std::vector<std::size_t> forward,back;

    if(size < 0 || index < 0 || index > size) return false;

    static std::map<std::pair<std::size_t,std::size_t>, std::vector<std::size_t> >
        forward_index, back_index;

    std::map<std::pair<std::size_t,std::size_t>, std::vector<std::size_t> >::const_iterator x;

    x = forward_index.find({index,size});
    if(x != forward_index.end()) {
        forward = x->second;
        back = back_index[{index,size}];
    } else {

        if(index < size/2){
            first = 0;
            last = index;
            inc = 1;
            first_b = index + 1;
            last_b = 2*index + 1;
            inc_b = +1;
        } else {
            first = size-1;
            last = index+1;
            inc = -1;
            first_b = index;                
            last_b = size - 2 *(size - (index+1));        // 7,3 -> 1; 7,4 -> 3; 7,5 -> 5
            inc_b = -1;
        }

        for(std::size_t i = first; i != last + inc; i += inc) forward.push_back(i);
        for(std::size_t i = last_b; i != first_b - inc_b; i -= inc_b) back.push_back(i);

        forward_index[{index,size}] = forward;
        back_index[{index,size}] = back;
    }

    if(false){
        std::cout << size << ' ' << index << '\n';
        for(const auto & x : forward) std :: cout << x << ' ' ;
        std::cout << '\n';
        for(const auto & x : back) std :: cout << x << ' ' ;
        std::cout << '\n';
    }

    bool answer = true;

    assert(forward.size() == back.size());

    for(std::size_t i = 0; i < forward.size(); ++i){
        std::string this_row = note[forward[i]], that_row = note[back[i]];
        assert(this_row.size() == that_row.size());
        for(std::size_t j = 0; j < this_row.size(); ++j) {
            if(this_row[j] != that_row[j]) {
                answer = false;
                break;
            }
        }
        if(!answer) break;
    }

    return answer;
}


auto is_reflected_v(const aoc::note& note, std::size_t index) -> bool
{
    std::size_t first, last, inc;
    std::size_t first_b, last_b, inc_b;
    std::size_t size = note[0].size();

    std::vector<std::size_t> forward,back;

    if(size < 0 || index < 0 || index > size) return false;

    static std::map<std::pair<std::size_t,std::size_t>, std::vector<std::size_t> >
        forward_index, back_index;

    std::map<std::pair<std::size_t,std::size_t>, std::vector<std::size_t> >::const_iterator x;

    x = forward_index.find({index,size});
    if(x != forward_index.end()) {
        forward = x->second;
        back = back_index[{index,size}];
    } else {

        if(index < size/2){
            first = 0;
            last = index;
            inc = 1;
            first_b = index + 1;
            last_b = 2*index + 1;
            inc_b = +1;
        } else {
            first = size-1;
            last = index+1;
            inc = -1;
            first_b = index;                
            last_b = size - 2 *(size - (index+1));        // 7,3 -> 1; 7,4 -> 3; 7,5 -> 5
            inc_b = -1;
        }

        for(std::size_t i = first; i != last + inc; i += inc) forward.push_back(i);
        for(std::size_t i = last_b; i != first_b - inc_b; i -= inc_b) back.push_back(i);

        forward_index[{index,size}] = forward;
        back_index[{index,size}] = back;
    }

    if(false){
        std::cout << size << ' ' << index << '\n';
        for(const auto & x : forward) std :: cout << x << ' ' ;
        std::cout << '\n';
        for(const auto & x : back) std :: cout << x << ' ' ;
        std::cout << '\n';
    }

    bool answer = true;

    assert(forward.size() == back.size());

    for(const auto & row : note)
    {
        for(std::size_t i = 0; i < forward.size(); i++) {
            if(row[forward[i]] != row[back[i]]) {
                answer = false;
                break;
            }
        }
        if(!answer) break;
    }

    return answer;
}
