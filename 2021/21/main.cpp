#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>


int main (int argc, char **argv)
{

    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    part_1(Context);

    /* 
    Part 1 approach won't scale for this...
    OK. 21 is a low score so should only need a few turns to track it. 
    So. Each turn the player will roll:
    3 = 1,1,1                                                   = 1
    4 = 2,1,1 | 1,2,1 | 1,1,2                                   = 3
    5 = 1,1,3 | 1,3,1 | 3,1,1 | 1,2,2 | 2,1,2 | 2,2,1           = 6  
    6 = 2,2,2 | 3,2,1 | 2,3,1 | 2,1,3 | 3,1,2 | 1,2,3 | 1,3,2   = 7
    7 = 3,3,1 | 3,1,3 | 1,3,3 | 3,2,2 | 2,3,2 | 2,2,3           = 6
    8 = 3,3,2 | 3,2,3 | 2,3,3                                   = 3
    9 = 3,3,3                                                   = 1

    so we can track game state as
    position 1, position 2, score_1, score_2, next_player 
    and have a map from this to ULL to track how many sequences of rolls end up in that state. 
    Don't need to track the board, just the two players (doesn't matter if they coincide...)

    */

   // try a dynamic programming approach - have to do a little thinking but elsewhere... 

    // pathological case is roll to land on 1, get 3x1 to land on 4, then land on 1, then ...
    // so score goes 1,4,5,8,9,12,13,16,17,20,21
    // i.e. 11th turn wins 

    std::vector<std::pair<uint64_t,uint64_t>> one_tracker(11, std::make_pair(0,0)),
        two_tracker(11,std::make_pair(0,0));

    dirac_tick(Context.start_1,one_tracker.begin(),one_tracker.end());
    dirac_tick(Context.start_2,two_tracker.begin(),two_tracker.end());

    // player 1 wins up to die roll x
    // is the same as p1 wins on roll x-1
    // plus wins on x
    // less rolls on x-1 where p2 actually won

    std::vector<uint64_t> p1_win_count(11,0), p2_win_count(11,0);

    for(std::size_t i = 1; i < 11; i++)
    {
        // number of winning scenarios: p2 must not win previous round, and p1 wins this round
        p1_win_count[i] = p1_win_count[i-1] + 
            one_tracker[i].first * two_tracker[i-1].second;

        // for p2, wins if p1 immediately loses
        p2_win_count[i] = p2_win_count[i-1] +
            one_tracker[i-1].second * two_tracker[i-1].first;
    }

    std::cout << std::max(p1_win_count.back(), p2_win_count.back()) << std::endl;


    return 0;

}