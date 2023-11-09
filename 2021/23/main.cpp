#include "parser.hpp"
#include "context.hpp"
#include "scanner.hpp"
#include "funcs.hpp"

int main (int argc, char **argv)
{

    aoc::scanner TokenScanner(std::cin, std::cout);
    aoc::context Context;
    aoc::parser Parser(TokenScanner,Context);

    Parser.parse();

    aoc::board initial;
    initial.pockets[0][0] = from_letter(Context.letters[0]);
    initial.pockets[1][0] = from_letter(Context.letters[1]);
    initial.pockets[2][0] = from_letter(Context.letters[2]);
    initial.pockets[3][0] = from_letter(Context.letters[3]);
    initial.pockets[0][1] = from_letter(Context.letters[4]);
    initial.pockets[1][1] = from_letter(Context.letters[5]);
    initial.pockets[2][1] = from_letter(Context.letters[6]);
    initial.pockets[3][1] = from_letter(Context.letters[7]);

    for(std::size_t i = 0; i < 4; i++) {
        initial.pockets[i][2] = initial.pockets[i][3] = aoc::state::unoccupiable;
    }

    std::cout << solve(initial) << std::endl;

    std::cout << initial;

    // part 2

    initial.pockets[0][0] = from_letter(Context.letters[0]);
    initial.pockets[1][0] = from_letter(Context.letters[1]);
    initial.pockets[2][0] = from_letter(Context.letters[2]);
    initial.pockets[3][0] = from_letter(Context.letters[3]);

    initial.pockets[0][1] = from_letter('D');
    initial.pockets[1][1] = from_letter('C');
    initial.pockets[2][1] = from_letter('B');
    initial.pockets[3][1] = from_letter('A');

    initial.pockets[0][2] = from_letter('D');
    initial.pockets[1][2] = from_letter('B');
    initial.pockets[2][2] = from_letter('A');
    initial.pockets[3][2] = from_letter('C');

    initial.pockets[0][3] = from_letter(Context.letters[4]);
    initial.pockets[1][3] = from_letter(Context.letters[5]);
    initial.pockets[2][3] = from_letter(Context.letters[6]);
    initial.pockets[3][3] = from_letter(Context.letters[7]);

    std::cout << solve(initial) << std::endl;

    std::cout << initial;

    return 0;

}
