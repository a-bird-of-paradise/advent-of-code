#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    int64_t answer_1 = 0, answer_2 = 0, counter = 0;

    for(const auto & note : Context.notes) {

        std::cout << ++counter << '\n';

        bool part_1_is_H=false, part_2_found  = false;
        std::size_t part_1_index;

        for(std::size_t h_idx = 0; h_idx < note.size()-1; ++h_idx) {
            if(is_reflected_h(note,h_idx)) {
                answer_1 += 100*(1 + h_idx);
                part_1_is_H = true;
                part_1_index = h_idx;
                break;
            }
        }
        if(!part_1_is_H) {
            for(std::size_t v_idx = 0; v_idx < note[0].size()-1; ++v_idx) {
                if(is_reflected_v(note,v_idx)) {
                    answer_1 += 1+v_idx;
                    part_1_index = v_idx;
                    break;
                }
            }
        }

        for(std::size_t smudge_i = 0; smudge_i < note.size(); ++smudge_i) {
                
            if(part_2_found) break;

            for(std::size_t smudge_j = 0; smudge_j < note[0].size(); ++smudge_j) {
                
                if(part_2_found) break;

                aoc::note smudged = note;
                char current = smudged[smudge_i][smudge_j];

                if (current == '.') {
                    smudged[smudge_i][smudge_j] = '#';
                } else if (current == '#') {
                    smudged[smudge_i][smudge_j] = '.';
                }

                for(std::size_t h_idx = 0; h_idx < smudged.size()-1; ++h_idx) {
                if(is_reflected_h(smudged,h_idx)
                    && (!part_1_is_H || (part_1_is_H && part_1_index != h_idx))) {
                    answer_2 += 100*(1 + h_idx);
                    part_2_found = true;
                    break;
                    }
                }

                if(!part_2_found) {
                    for(std::size_t v_idx = 0; v_idx < smudged[0].size()-1; ++v_idx) {
                        if(is_reflected_v(smudged,v_idx)
                            && (part_1_is_H || (!part_1_is_H && part_1_index != v_idx))) {
                            answer_2 += 1+v_idx;
                            part_2_found = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    std::cout << answer_1 << '\n' << answer_2 << '\n';

    return 0;
}
