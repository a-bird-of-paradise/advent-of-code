#include "astnode.hpp"
#include "Parser.hpp"

// anything too hairy for a header definition goes in here

namespace AST {

    std::unique_ptr<AST_Node> single_or_list(std::unique_ptr<AST_Node>&& l, std::unique_ptr<AST_Node>&& r)
    {
        if(r.get() == nullptr) return std::move(l);
        return std::make_unique<List_Node>(std::move(l),std::move(r));
    }

    User_Node::User_Node(Symbol *s, std::unique_ptr<AST_Node>&& v, cppcalc::Context *ctx) : 
        m_s(s), m_v(std::move(v)), m_ctx(ctx)
    {
        // check name defined as function
        if(m_s->func.get() == nullptr) 
            throw cppcalc::Parser::syntax_error(m_ctx->loc,
                "Symbol " + m_s->Name + " not defined as a function");

        // check enough args and store new local scope values

        std::size_t n_args = m_s->symlist.size();
        AST_Node *current = m_v.get();
        for(std::size_t i = 0; i < n_args; i++){
            // m_v is either a list of expressions, or a singleton expression, or empty (i.e. insufficient args)
            if(current == nullptr) 
                throw cppcalc::Parser::syntax_error(m_ctx->loc,"Insufficient arguments");
            bool is_list = dynamic_cast<List_Node*>(current) != nullptr ? true : false;
            if(is_list) {
                current = dynamic_cast<List_Node*>(current)->m_right.get();
            } else {
                current = nullptr;
            }
        }
    }

    double User_Node::eval() {

    std::vector<double> old_values, new_values;
    
    // check enough args and store new local scope values

    std::size_t n_args = m_s->symlist.size();
    AST_Node *current = m_v.get();
    for(std::size_t i = 0; i < n_args; i++){
        // m_v is either a list of expressions, or a singleton expression, or empty (i.e. insufficient args)
        if(current == nullptr) 
            throw cppcalc::Parser::syntax_error(m_ctx->loc,"Insufficient arguments");
        bool is_list = dynamic_cast<List_Node*>(current);
        if(is_list) {
            new_values.push_back(dynamic_cast<List_Node*>(current)->m_left->eval());
            current = dynamic_cast<List_Node*>(current)->m_right.get();
        } else {
            new_values.push_back(current->eval());
            current = nullptr;
        }
    }

    // enough args, so save old global scope values and store new local scope values in symbols 

    for(std::size_t i = 0; i < n_args; i++){
        Symbol *sym = m_s->symlist[i];
        old_values.push_back(sym->Value);
        sym->Value = new_values[i];
    }

    // evaluate function in local context

    double answer = m_s->func->eval();

    // restore global context

    for(std::size_t i = 0; i < n_args; i++){
        Symbol *sym = m_s->symlist[i];
        sym->Value = old_values[i];
    }

    // done

    return answer;
    }

    Builtin_Node::Builtin_Node(Builtin_Function func, std::unique_ptr<AST_Node>&& v, cppcalc::Context *ctx) 
        : 
        m_func(func), m_v(std::move(v)), m_ctx(ctx) {}
    double Builtin_Node::eval() {
        double answer = m_v->eval();
        switch(m_func){
            case Builtin_Function::B_ABS: answer = fabs(answer); break;
            case Builtin_Function::B_DEBUG: answer = 0; break;
            case Builtin_Function::B_EXP: answer = exp(answer); break;
            case Builtin_Function::B_LOG: answer = log(answer); break;
            case Builtin_Function::B_PRINT: break;
            case Builtin_Function::B_QUIT: m_ctx->done = true; break;
            case Builtin_Function::B_SQRT: answer = sqrt(answer); break;
        }
        return answer;
    }

    Conditional_Node::Conditional_Node(std::unique_ptr<AST_Node>&& cond, 
                                        std::unique_ptr<AST_Node>&& then_branch, 
                                        std::unique_ptr<AST_Node>&& else_branch) :
        m_cond(std::move(cond)), 
        m_then_branch(std::move(then_branch)),
        m_else_branch(std::move(else_branch)) 
        {}

    If_Node::If_Node(std::unique_ptr<AST_Node>&& cond, 
                    std::unique_ptr<AST_Node>&& then_branch,
                    std::unique_ptr<AST_Node>&& else_branch) :
        Conditional_Node(std::move(cond), std::move(then_branch),std::move(else_branch)) {}

    double If_Node::eval() {
        double answer = 0;
        if(m_cond->eval() != 0) { // true
            answer = m_then_branch->eval();
        }
        else {
            if(m_else_branch.get() != nullptr) {
                answer = m_else_branch->eval();
            }
        }
        return answer;
    }

    While_Node::While_Node(std::unique_ptr<AST_Node>&& cond, 
                            std::unique_ptr<AST_Node>&& then_branch, 
                            std::unique_ptr<AST_Node>&& else_branch) :
        Conditional_Node(std::move(cond), std::move(then_branch),std::move(else_branch)) {}
    double While_Node::eval() {
        double answer = 0;
        while(m_cond->eval() != 0) answer = m_then_branch->eval();
        return answer;
    }

    Ref_Node::Ref_Node(Symbol *s) : m_s(s) {}
    double Ref_Node::eval() { return m_s->Value; }

    Assign_Node::Assign_Node(Symbol *s, std::unique_ptr<AST_Node>&& v) : m_s(s), m_v(std::move(v)) {}
    double Assign_Node::eval() { return m_s->Value = m_v->eval(); }
}