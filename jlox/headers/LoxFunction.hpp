#ifndef LOXFUNCTION_HPP
#define LOXFUNCTION_HPP

#include "memory"
#include "vector"
#include "string"

#include "LoxCallable.hpp"
#include "Visitor.hpp"
#include "Environment.hpp"

class LoxFunction : public LoxCallable {
    private:
        std::shared_ptr<const Stmt> declaration;
        env_ptr closure;
        bool isInitializer;

    public:
        LoxFunction(const Function* declaration, env_ptr& closure, bool isInitializer);
        LoxFunction(const stmt_ptr& declaration, env_ptr& closure, bool isInitializer);
        call_ptr bind(LoxInstance instance);
        std::string toString() override;
        int arity() override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
};

#endif