#include "funcs.hpp"
#include "scanner.hpp"

void aoc::parser::error(aoc::location const& loc, std::string const& msg)
{
    std::cerr << loc << ": " << msg << std::endl;
}

void part_1(const aoc::context& Context)
{
    aoc::DeterministicDie d_die;

    int p1_pos = Context.start_1, p2_pos = Context.start_2;

    int next_player = 1, current_index;

    unsigned long long player_1_score = 0, player_2_score=0;

    while(player_1_score < 1000 && player_2_score < 1000) 
    {
        int three_roll_sum = d_die.roll() + d_die.roll() + d_die.roll();

        if(next_player == 1) {
            current_index = p1_pos - 1;
            current_index += three_roll_sum;
            current_index %= 10;
            p1_pos = current_index + 1;
            player_1_score += p1_pos;
        } else {
            current_index = p2_pos - 1;
            current_index += three_roll_sum;
            current_index %= 10;
            p2_pos = current_index + 1;
            player_2_score += p2_pos;
        }

        next_player = (next_player + 1) % 2;
    }

    if(player_1_score >= 1000) {
        std::cout << player_2_score * d_die.roll_count() << std::endl;
    } else {
        std::cout << player_1_score * d_die.roll_count() << std::endl;
    }
}

void dirac_tick(int current_position, 
    std::vector<std::pair<uint64_t,uint64_t>>::iterator start,
    std::vector<std::pair<uint64_t,uint64_t>>::const_iterator end,
    int to_go,
    uint64_t scenarios) {

    static const std::vector<std::pair<uint8_t,uint8_t>> frequencies = 
        { {3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1} };

    if(start == end) return;

    for(auto const & [roll, freq] : frequencies) {

        int next_pos = current_position + roll;
        if(next_pos > 10) next_pos -= 10;
        uint64_t num_scenarios = scenarios * freq;

        if(next_pos >= to_go) {
            start->first += num_scenarios;
        } else {
            start->second += num_scenarios;
            dirac_tick(next_pos, std::next(start), end, to_go - next_pos, num_scenarios);
        }
    }
}