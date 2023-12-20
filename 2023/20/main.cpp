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

    std::queue<aoc::pulse> pulse_queue, next_pulse_queue;
    std::map<bool,int64_t> counter;

    int64_t ticker = 0, answer_2 = 0, answer_1 = 0;
    bool seen_answer_2 = false, test, has_rx;

    has_rx = Context.modules.contains("rx");

    while(ticker < 5000)
    {
        ticker++;
        pulse_queue.push({"BUTTON","broadcaster",false});
        counter[false]++;

        while(!pulse_queue.empty()) {
            test = process_queue(pulse_queue,next_pulse_queue,Context.modules,counter);
            pulse_queue = std::move(next_pulse_queue);
            while(!next_pulse_queue.empty()) next_pulse_queue.pop();
            if(test) std::cout << ticker << '\n';
        }

        if(ticker == 1000) answer_1 = counter[false]*counter[true];

    }
    std::cout << answer_1 << '\n' << answer_2 << '\n';

    std::cout << (3767LL) * (3931LL) * (3923LL) * (4007LL) << '\n';

    return 0;
}

// fp 3767
// mk 3931
// zc 3923
// xt 4007
// 233131004673731 too high ... 
// 232774988886497