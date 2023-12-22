#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <map>
#include <set>
#include <queue>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    int64_t min_x, min_y, min_z, max_x, max_y, max_z;

    min_x = min_y = min_z = INT64_MAX;
    max_x = max_y = max_z = INT64_MIN;

    for(const auto & brick : Context.bricks) {
        min_x = std::min(brick.start_x, std::min(brick.end_x, min_x));
        min_y = std::min(brick.start_y, std::min(brick.end_y, min_y));
        min_z = std::min(brick.start_z, std::min(brick.end_z, min_z));
        max_x = std::max(brick.start_x, std::max(brick.end_x, max_x));
        max_y = std::max(brick.start_y, std::max(brick.end_y, max_y));
        max_z = std::max(brick.start_z, std::max(brick.end_z, max_z));
    }

    std::vector<std::vector<std::vector<int16_t>>> space;

    space.resize(max_x+1);
    for(auto&& x : space) x.resize(max_y+1);
    for(auto&& x : space) for (auto&& y : x) y.resize(max_z+1);
    for(auto&& x : space) for (auto&& y : x) for(auto&& z : y) z = 0;

    for(auto b = 1; b <= Context.bricks.size(); ++b)
        for(auto i = std::min(Context.bricks[b-1].start_x,Context.bricks[b-1].end_x); 
                i <= std::max(Context.bricks[b-1].start_x,Context.bricks[b-1].end_x); 
                ++i)
            for(auto j = std::min(Context.bricks[b-1].start_y,Context.bricks[b-1].end_y); 
                    j <= std::max(Context.bricks[b-1].start_y,Context.bricks[b-1].end_y); 
                    ++j)
                for(auto k = std::min(Context.bricks[b-1].start_z,Context.bricks[b-1].end_z); 
                        k <= std::max(Context.bricks[b-1].start_z,Context.bricks[b-1].end_z); 
                        ++k)
                    space[i][j][k]=b;
    

    bool any_moved = true;

    std::vector<aoc::brick> current_locs = Context.bricks;

    while(any_moved)
    {
        any_moved = false;
        // drop brick if we can 
        for(auto b = 1; b <= current_locs.size(); ++b) {

            if(std::min(current_locs[b-1].start_z, current_locs[b-1].end_z) == 0) continue;

            bool free_below = true;

            // can drop? 

            for(auto x = std::min(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                    x <= std::max(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                    ++x) {
                for(auto y = std::min(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                        y <= std::max(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                        ++y) {
                    if(space[x][y][std::min(current_locs[b-1].start_z, current_locs[b-1].end_z)-1] != 0) {
                        free_below = false;
                         break;
                    }
                }
                if(!free_below) break;
            }

            // do drop 
  
            if(free_below) {
                
                any_moved = true;

                // clear old          

                for(auto x = std::min(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                        x <= std::max(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                        ++x) {
                    for(auto y = std::min(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                            y <= std::max(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                            ++y) {
                        for(auto z = std::min(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                                z <= std::max(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                                ++z) {
                            space[x][y][z] = 0;
                        }
                    }
                }

                // update position 

                current_locs[b-1].start_z--;
                current_locs[b-1].end_z--;

                // write new

                for(auto x = std::min(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                        x <= std::max(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                        ++x) {
                    for(auto y = std::min(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                            y <= std::max(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                            ++y) {
                        for(auto z = std::min(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                                z <= std::max(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                                ++z) {
                            space[x][y][z] = b;
                        }
                    }
                }
            }
        }
    }

    std::map<int,std::set<int>> supports, is_supported_by;

    for(auto b = 1; b <= current_locs.size(); ++b)
    {
        supports[b];
        is_supported_by[b];
        for(auto x = std::min(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                x <= std::max(current_locs[b-1].start_x,Context.bricks[b-1].end_x); 
                ++x) {
            for(auto y = std::min(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                    y <= std::max(current_locs[b-1].start_y,current_locs[b-1].end_y); 
                    ++y) {
                for(auto z = std::min(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                        z <= std::max(current_locs[b-1].start_z,current_locs[b-1].end_z); 
                        ++z) {
                    if(z >= space[x][y].size()-1) continue;
                    if(space[x][y][z+1] != 0 && space[x][y][z+1] != b){ 
                        supports[b].insert(space[x][y][z+1]);
                        is_supported_by[space[x][y][z+1]].insert(b);
                    }
                }
            }
        }
    }

    // part 1

    int16_t answer_1 = 0;

    std::set<int16_t> can_be_dropped;

    for(auto b = 1; b <= current_locs.size(); ++b)
    {

        int16_t least_other_supporters = INT16_MAX;

        for(const auto & supported : supports[b]) {
            least_other_supporters = std::min<int16_t>(least_other_supporters, is_supported_by[supported].size()-1);
        }

        if(least_other_supporters > 0)
            answer_1++;
    }

    std::cout << answer_1 << '\n';

    // part 2

    int64_t answer_2 = 0;

    std::queue<int16_t> to_drop;

    std::map<int,std::set<int>> is_supported_by_2 ;

    for(auto b = 1; b <= current_locs.size(); ++b)
    {
        is_supported_by_2 = is_supported_by;

        to_drop.push(b);

        while(!to_drop.empty())
        {
            int16_t dropped = to_drop.front();
            to_drop.pop();

            for(const auto& supported : supports[dropped]) {
                is_supported_by_2[supported].erase(dropped);
                if(is_supported_by_2[supported].empty()) {
                    to_drop.push(supported);
                    answer_2++;
                }
            }
        }
    }

    std::cout << answer_2 << '\n';

    return 0;
}