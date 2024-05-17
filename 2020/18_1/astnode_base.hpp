#pragma once

namespace AST {

    class AST_Node
    {
    public:
        virtual ~AST_Node() {}
        virtual double eval() = 0;
    };
}