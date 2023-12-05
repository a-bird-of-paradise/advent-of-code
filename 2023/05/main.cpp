#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"
#include <algorithm>

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    // part 1

    std::vector<long long> soil, fertiliser, water, light, temperature, humidity, location;

    soil = apply_map(Context.seeds,Context.seed_to_soil);
    fertiliser = apply_map(soil,Context.soil_to_fertiliser);
    water = apply_map(fertiliser,Context.fertiliser_to_water);
    light = apply_map(water,Context.water_to_light);
    temperature = apply_map(light,Context.light_to_temperature);
    humidity = apply_map(temperature,Context.temperature_to_humidity);
    location = apply_map(humidity,Context.humidity_to_location);

    std::cout << *std::ranges::min_element(location) << '\n';

    // part 2, just use brute force

    std::vector<aoc::segment> segments;

    for(std::size_t i = 0; i < Context.seeds.size(); i += 2)
        segments.emplace_back(Context.seeds[i],Context.seeds[i+1]);

    long long min_seen = INT64_MAX, temp;

    for(const auto& s : segments){
        for(long long i = 0; i < s.second; i++) {
            temp = apply_map_single(s.first+i,Context.seed_to_soil);
            temp = apply_map_single(temp,Context.soil_to_fertiliser);
            temp = apply_map_single(temp,Context.fertiliser_to_water);
            temp = apply_map_single(temp,Context.water_to_light);
            temp = apply_map_single(temp,Context.light_to_temperature);
            temp = apply_map_single(temp,Context.temperature_to_humidity);
            temp = apply_map_single(temp,Context.humidity_to_location);

            min_seen = temp < min_seen ? temp : min_seen;
        }
    }

    std::cout << min_seen << '\n';

    return 0;

}
