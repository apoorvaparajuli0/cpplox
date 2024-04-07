#ifndef LOXFUNCTION_HPP
#define LOXFUNCTION_HPP

#include "Environment.hpp"
#include "Stmt.hpp"
#include "RuntimeError.hpp"

class LoxFunction : public LoxCallable {
    private:
        std::shared_ptr<const Stmt> declaration;
        env_ptr closure;
        bool isInitializer;

    public:
        //not sure if this actually sets our stmt_ptr correctly
        LoxFunction(const Function* declaration, env_ptr& closure, bool isInitializer);
        call_ptr bind(LoxInstance instance);
        std::string toString() override;
        int arity() override;
        Object call(Interpreter& interpreter, std::vector<Object>& arguments) override;
};

#endif