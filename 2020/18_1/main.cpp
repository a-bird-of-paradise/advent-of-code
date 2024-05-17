#include "Scanner.hpp"
#include "Parser.hpp"

int main(int argc, char **argv)
{

    cppcalc::Context ctx;
    cppcalc::Scanner scanner(std::cin,std::cerr);
    cppcalc::Parser parser(&scanner,&ctx);

    int64_t answer_1 = 0;

    while(!ctx.done)
    {
        parser.parse();
        if(ctx.node.get()) answer_1 += ctx.node->eval();
    }

    std::cout << answer_1 << '\n';

    return 0;

}