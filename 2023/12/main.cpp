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

    // part 1

    int64_t answer_1 = 0;

    for(std::size_t i = 0; i < Context.field.size(); ++i) {
        std::string x;
        x.insert(x.end(),std::begin(Context.field[i]),std::end(Context.field[i]));
        answer_1 += recurse(x,Context.contiguous[i]);
    }

    std::cout << answer_1 << '\n';

    // part 2

    int64_t answer_2 = 0;

    for(std::size_t i = 0; i < Context.field.size(); ++i)
    {

        std::string field; 
        field.insert(field.end(),std::begin(Context.field[i]),std::end(Context.field[i]));
        field.push_back('?');
        field.insert(field.end(),std::begin(Context.field[i]),std::end(Context.field[i]));
        field.push_back('?');
        field.insert(field.end(),std::begin(Context.field[i]),std::end(Context.field[i]));
        field.push_back('?');
        field.insert(field.end(),std::begin(Context.field[i]),std::end(Context.field[i]));
        field.push_back('?');
        field.insert(field.end(),std::begin(Context.field[i]),std::end(Context.field[i]));

        std::vector<int64_t> report = Context.contiguous[i];
        report.insert(report.end(),std::begin(Context.contiguous[i]),std::end(Context.contiguous[i]));
        report.insert(report.end(),std::begin(Context.contiguous[i]),std::end(Context.contiguous[i]));
        report.insert(report.end(),std::begin(Context.contiguous[i]),std::end(Context.contiguous[i]));
        report.insert(report.end(),std::begin(Context.contiguous[i]),std::end(Context.contiguous[i]));

        answer_2 += recurse(field,report);

    }

    std::cout << answer_2 << '\n';

    return 0;
}
