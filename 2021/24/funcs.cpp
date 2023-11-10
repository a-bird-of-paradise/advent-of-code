#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

std::vector<long long> precdents(long long w, long long A, long long B, long long d, long long z)
{
    std::vector<long long> answer;

    long long x = z - w - B;
    long long adj = x < 0 ? 1 : 0;
    if(x % 26 == 0) answer.push_back( (x/26 + adj) * d );
    if(0 <= w-A && w-A < 26) {
        z *= d;
        answer.push_back(w - A + z);
    }
    return answer;
}

bool cmp::operator()(const std::vector<long long>& a,const std::vector<long long>& b) const
{ 
    auto a_it = a.rbegin();
    auto b_it = b.rbegin();

    while(a_it != a.rend() && b_it != b.rend())
    {
        if(*a_it == *b_it) {
            a_it++;
            b_it++;
            continue;
        }
        return *a_it < *b_it;
    }
    return false;
}

void dfs(const aoc::context& ctx,
        std::vector<long long>& current_number,
        std::set<std::vector<long long>, cmp>& valid_numbers,
        std::size_t depth,
        long long z_n)
{
    for(long long next = 1; next < 10; next++){

        std::vector<long long> valid_z = precdents(next,ctx.A[depth-1],ctx.B[depth-1],ctx.d[depth-1],z_n);
       
        if(depth-1 == 0) {
            if(std::find(valid_z.begin(),valid_z.end(),0) != valid_z.end()) {
                current_number.push_back(next);
                valid_numbers.insert(current_number);
                current_number.pop_back();
            }
        } else {
            for(const auto & z : valid_z) {
                current_number.push_back(next);
                dfs(ctx,current_number,valid_numbers,depth-1,z);
                current_number.pop_back();
            }
        }
    }
    return;
}