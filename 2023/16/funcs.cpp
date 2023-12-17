#include "funcs.hpp"
#include "scanner.hpp"
#include "context.hpp"
#include <map>
#include <algorithm>

auto aoc::parser::error(aoc::location const& loc, std::string const& msg) -> void
{
    std::cerr << loc << ": " << msg << std::endl;
}

auto path_length(const std::vector<std::string>& field, const aoc::beam& start_beam) -> int32_t
{
    std::vector<aoc::beam> beams, new_beams;
    beams.push_back(start_beam);

    std::map<aoc::point, std::vector<aoc::point>> graph;

    while(!beams.empty())
    {
        for(const auto& beam : beams)
        {
            graph[beam.p];

            switch(field[beam.p.r][beam.p.c])
            {
                case '.':
                    switch(beam.dir)
                    {
                        case aoc::direction::down:
                            if(beam.p.r + 1 < field.size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r+1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r+1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r+1,beam.p.c});
                            }
                            break;
                        case aoc::direction::up:
                            if(beam.p.r - 1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r-1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r-1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r-1,beam.p.c});
                            }
                            break;
                        case aoc::direction::left:
                            if(beam.p.c - 1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c-1}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r,beam.p.c-1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c-1});
                            }
                            break;
                        case aoc::direction::right:
                            if(beam.p.c + 1 < field[beam.p.r].size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c+1}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r,beam.p.c+1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c+1});
                            }
                            break;
                    }
                    break;

                case '-':
                    switch(beam.dir)
                    {
                        case aoc::direction::down:
                        case aoc::direction::up:
                            // left
                            if(beam.p.c - 1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c-1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::left,beam.p.r,beam.p.c-1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c-1});
                            }
                            // right
                            if(beam.p.c + 1 < field[beam.p.r].size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c+1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::right,beam.p.r,beam.p.c+1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c+1});
                            }
                            break;
                        case aoc::direction::left:
                            if(beam.p.c -1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c-1}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r,beam.p.c-1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c-1});
                            }
                            break;
                        case aoc::direction::right:
                            if(beam.p.c + 1 < field[beam.p.r].size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c+1}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r,beam.p.c+1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c+1});
                            }
                            break;
                    }
                    break;

                case '|':
                    switch(beam.dir)
                    {
                        case aoc::direction::down:
                            if(beam.p.r + 1 < field.size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r+1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r+1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r+1,beam.p.c});
                            }
                            break;
                        case aoc::direction::up:
                            if(beam.p.r -1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r-1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({beam.dir,beam.p.r-1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r-1,beam.p.c});
                            }
                            break;
                        case aoc::direction::left:
                        case aoc::direction::right:
                            // up
                            if(beam.p.r - 1 >= 0  &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r-1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::up,beam.p.r-1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r-1,beam.p.c});
                            }
                            // down
                            if(beam.p.r + 1 < field.size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r+1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::down,beam.p.r+1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r+1,beam.p.c});
                            }
                            break;
                    }
                    break;
                case '/':
                    switch(beam.dir)
                    {
                        case aoc::direction::down:
                            if(beam.p.c - 1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c-1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::left,beam.p.r,beam.p.c-1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c-1});
                            }
                            break;
                        case aoc::direction::up:
                            if(beam.p.c + 1 < field[beam.p.r].size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c+1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::right,beam.p.r,beam.p.c+1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c+1});
                            }
                            break;
                        case aoc::direction::left:
                            if(beam.p.r + 1 < field.size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r+1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::down,beam.p.r+1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r+1,beam.p.c});
                            }
                            break;
                        case aoc::direction::right:
                            if(beam.p.r -1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r-1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::up,beam.p.r-1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r-1,beam.p.c});
                            }
                            break;
                    }
                    break;
                case '\\':
                    switch(beam.dir)
                    {
                        case aoc::direction::up:
                            if(beam.p.c - 1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c-1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::left,beam.p.r,beam.p.c-1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c-1});
                            }
                            break;
                        case aoc::direction::down:
                            if(beam.p.c + 1 < field[beam.p.r].size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r,beam.p.c+1}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::right,beam.p.r,beam.p.c+1});
                                    graph[beam.p].push_back({beam.p.r,beam.p.c+1});
                            }
                            break;
                        case aoc::direction::left:
                            if(beam.p.r -1 >= 0 &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r-1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::up,beam.p.r-1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r-1,beam.p.c});
                            }
                            break;
                        case aoc::direction::right:
                            if(beam.p.r + 1 < field.size() &&
                                std::ranges::find(graph[beam.p], aoc::point{beam.p.r+1,beam.p.c}) == graph[beam.p].end()) {
                                    new_beams.push_back({aoc::direction::down,beam.p.r+1,beam.p.c});
                                    graph[beam.p].push_back({beam.p.r+1,beam.p.c});
                            }
                            break;
                    }
                    break;
                default:
                    std::cout << field[beam.p.r][beam.p.c] << '\n'; assert(false);
            }

        }

        beams = std::move(new_beams);
        new_beams.clear();
    }
    return graph.size();
}
