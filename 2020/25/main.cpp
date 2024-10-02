#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

auto main() -> int
{
    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser();

    std::cout << Context.key_1 << ' ' << Context.key_2 << '\n';

    int loop_1 = 0, loop_2 = 0;
    uint64_t transformed_key_1 = 1, transformed_key_2 = 1;
    const uint64_t subject = 7, divisor = 20201227;

    while(transformed_key_1 != Context.key_1) {
        loop_1++;
        transformed_key_1 *= subject;
        transformed_key_1 %= divisor;
    }

    while(transformed_key_2 != Context.key_2) {
        loop_2++;
        transformed_key_2 *= subject;
        transformed_key_2 %= divisor;
    }

    std::cout << loop_1 << ' ' << loop_2 << '\n';

    transformed_key_1 = 1;
    transformed_key_2 = 1;
    
    for(uint64_t i = 0; i < loop_1; ++i) {
        transformed_key_1 *= Context.key_2;
        transformed_key_1 %= divisor;
    }
    
    for(uint64_t i = 0; i < loop_2; ++i) {
        transformed_key_2 *= Context.key_1;
        transformed_key_2 %= divisor;
    }
    
    std::cout << transformed_key_1 << ' ' << transformed_key_2 << '\n';

    return 0;
}