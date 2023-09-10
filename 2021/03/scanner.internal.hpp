#pragma once
//#include "Parser.hpp"

namespace aoc {
    class scanner : public aocFlexLexer {
    public:
        scanner(std::istream& arg_yyin, std::ostream& arg_yyout)
            : aocFlexLexer(arg_yyin, arg_yyout), column(0), row(0) {}
        scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr)
            : aocFlexLexer(arg_yyin, arg_yyout), column(0), row(0) {}
        int lex(void);

        
        int column, row;
        std::vector<std::string> rows;
    };
}
