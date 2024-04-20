#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <algorithm>
#include <format>

auto get_zero_mask(const std::string& mask) -> uint64_t
{
    uint64_t answer = UINT64_MAX,  val = 1; 

    for(auto x = mask.rbegin(); x != mask.rend(); ++x)
    {
        if(*x == '0')   answer -= val;
        val *= 2;
    }
    return answer;
}

auto get_ones_mask(const std::string& mask) -> uint64_t
{
    uint64_t answer = 0, val = 1; 

    for(auto x = mask.rbegin(); x != mask.rend(); ++x)
    {
        if(*x == '1')   answer += val;
        val *= 2;
    }
    return answer;
}

auto get_floats(const std::string& mask) -> uint64_t
{
    uint64_t answer = 0, val = 1;

    for(auto x = mask.rbegin(); x != mask.rend(); ++x)
    {
        if(*x == 'X') answer += val;
        val *= 2;
    }
    return answer;
}

auto get_addresses(uint64_t to_write, uint64_t float_mask) -> std::vector<uint64_t>
{
    std::vector<uint64_t> pre_loop, post_loop;

    pre_loop.push_back(to_write);

    for(std::size_t i = 0; i < 36; ++i){

        uint64_t mask_bit = 1ULL << static_cast<uint64_t>(i); 
        uint64_t not_mask = ~ mask_bit;
        

        if( (mask_bit & float_mask) == mask_bit)
        {
            post_loop.clear();
            for(const auto& addr : pre_loop)
            {
                post_loop.push_back(addr | mask_bit);
                post_loop.push_back(addr & not_mask); 
            }
            pre_loop = post_loop;
        }
    }

    return post_loop; 
}