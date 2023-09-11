#pragma once
#include "context.hpp"
#include "parser.hpp"

namespace aoc {
    class scanner : public aocFlexLexer {
    public:
        scanner(std::istream& arg_yyin, std::ostream& arg_yyout)
            : aocFlexLexer(arg_yyin, arg_yyout) {}
        scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr)
            : aocFlexLexer(arg_yyin, arg_yyout) {}
        parser::symbol_type lex(context& Context);

    };
}
