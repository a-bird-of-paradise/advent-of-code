#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>
#include <map> 

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    std::set<aoc::move> black_tiles, black_tiles_new, all_neighbours, only_white_neighbours;
    std::map<aoc::move,int> neighbour_count;

    black_tiles = Context.flipped;

    for(int i = 1; i <= 100; ++i) {
        for(const auto & move : black_tiles) {
            all_neighbours.insert(aoc::move{ 1 ,0} + move);
            all_neighbours.insert(aoc::move{ 0,-1} + move);
            all_neighbours.insert(aoc::move{-1,-1} + move);
            all_neighbours.insert(aoc::move{-1, 0} + move);
            all_neighbours.insert(aoc::move{ 0, 1} + move);
            all_neighbours.insert(aoc::move{ 1, 1} + move);
        }
        
        std::set_difference(
            std::begin(all_neighbours),
            std::end(all_neighbours),
            std::begin(black_tiles),
            std::end(black_tiles),
            std::inserter(only_white_neighbours,std::begin(only_white_neighbours))
        );

        // for each black tile, count black neighbours 

        for(const auto & move : black_tiles)
        {
            aoc::move current;
            current = aoc::move{ 1, 0} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 0,-1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{-1,-1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{-1, 0} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 0, 1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 1, 1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
        }

        // stick 1 or 2 neighbour'd tiles into the new set

        for(const auto & x : neighbour_count)
            if(x.second == 1 || x.second == 2)
                black_tiles_new.insert(x.first);
 
        // white tiles. clear maap
        neighbour_count.clear();
        for(const auto & move : only_white_neighbours)
        {
            aoc::move current;
            current = aoc::move{ 1, 0} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 0,-1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{-1,-1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{-1, 0} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 0, 1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
            current = aoc::move{ 1, 1} + move;  if(black_tiles.contains(current)) neighbour_count[move]++;
        }

        // stick 2 neighbour'd tiles into the new set

        for(const auto & x : neighbour_count)
            if(x.second == 2)
                black_tiles_new.insert(x.first);

        black_tiles = black_tiles_new;
        black_tiles_new.clear();
        neighbour_count.clear();
        all_neighbours.clear();
        only_white_neighbours.clear();
    }
    std::cout << black_tiles.size() << '\n';
    return 0;
}