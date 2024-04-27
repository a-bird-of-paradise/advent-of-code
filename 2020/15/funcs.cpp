#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <map>

auto do_sim(const std::vector<int64_t>& start, const int64_t n) -> int64_t
{

    std::map<int64_t, std::vector<int64_t>> data;

    int64_t counter = 0, last;

    while(counter < n)
    {
        // initially, just utter the start sequence
        if(counter < start.size())
        {
            data[start[counter]].push_back(counter);
            last = start[counter];
        }
        // now, consider the last thing spoken 
        else
        {
            auto it = data.find(last);
            if(it == data.end())
            {
                assert(false);
            }
            // first time seen it?
            else if(it->second.size() == 1)
            {
                data[0].push_back(counter);
                last = 0;
            }
            // second or more time seen it?
            else 
            {
                int64_t at_back = data[last].back();
                int64_t almost_at_back = *std::next(data[last].rbegin());
                int64_t age = at_back - almost_at_back;

                data[age].push_back(counter);
                last = age;
            }
        }
        ++counter;
        //if(counter % 10000 == 0) std::cout << counter << '\n';
    }
    return last;
}