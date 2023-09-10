#include "scanner.hpp"
#include <vector>

int main(int argc, char **argv)
{
    aoc::scanner Scanner(std::cin, std::cout);

    while(Scanner.lex());

    std::vector<int> one_counter(Scanner.column,0);

    for(auto &i : Scanner.rows) {
        for(int j = 0; j < i.length(); j++) {
            if ( i[j] == '1' ) one_counter[j]++;
        }
    }

    unsigned gamma = 0, epsilon = 0;

    for(auto &i : one_counter) {
        gamma <<= 1; 
        epsilon <<= 1;

        if (i > Scanner.rows.size() / 2) {
            gamma |= 1;
        } else {
            epsilon |= 1;
        }
    }
    
    std::cout << gamma * epsilon << std::endl;

    std::vector<std::string> oxy_candidates = Scanner.rows, co2_candidates = Scanner.rows;

    for(int i = 0; i < Scanner.column; i++)
    {
        int how_many_ones = 0, how_many_total = 0, how_many_zeroes = 0;
        char most_common = '0';
        if(oxy_candidates.size() == 1) break;

        for(auto &j : oxy_candidates)
        {
            how_many_total++;
            if(j[i] == '1') how_many_ones++;
        }
        how_many_zeroes = how_many_total - how_many_ones;
        if(how_many_ones >= how_many_zeroes) most_common = '1';

        oxy_candidates.erase(
            std::remove_if(
                oxy_candidates.begin(),
                oxy_candidates.end(),
                [i,most_common](const std::string& x) { return x[i] != most_common; }
            ),
            oxy_candidates.end()
        );
    }

    for(int i = 0; i < Scanner.column; i++)
    {
        int how_many_ones = 0, how_many_total = 0, how_many_zeroes = 0;
        char most_common = '1';
        if(co2_candidates.size() == 1) break;

        for(auto &j : co2_candidates)
        {
            how_many_total++;
            if(j[i] == '1') how_many_ones++;
        }
        how_many_zeroes = how_many_total - how_many_ones;
        if(how_many_zeroes > how_many_ones) most_common = '0';

        co2_candidates.erase(
            std::remove_if(
                co2_candidates.begin(),
                co2_candidates.end(),
                [i,most_common](const std::string& x) { return x[i] == most_common; }
            ),
            co2_candidates.end()
        );
    }

    unsigned long oxy, co2;
    oxy = std::stoul(oxy_candidates[0], nullptr, 2);
    co2 = std::stoul(co2_candidates[0], nullptr, 2);

    std::cout << oxy*co2 << std::endl;

    return 0;
}