#ifndef LOXFUNCTION_HPP
#define LOXFUNCTION_HPP

#include "LoxCallable.hpp"
#include "Visitor.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"
#include "Interpreter.hpp"
#include "ReturnError.hpp"

class LoxFunction : public LoxCallable {
    private:
        stmt_ptr declaration;
        env_ptr closure;

    public:
        //not sure if this actually sets our stmt_ptr correctly
        LoxFunction(const Function& declaration, env_ptr& closure) {
            this->declaration = stmt_ptr(new Function(declaration));
            this->closure = closure;
        }

        std::string toString() override {
            return "<fn" + dynamic_cast<Function*>(declaration.get())->name.lexeme + ">";
        }

        int arity() override {
            return dynamic_cast<Function*>(declaration.get())->params.size();
        }

        Object call(Interpreter& interpreter, std::vector<Object> arguments) override {
            env_ptr environment = env_ptr(new Environment(closure));

            for (int i = 0; i < dynamic_cast<Function*>(declaration.get())->params.size(); i++) {
                //might want to do sequence of ifs casting to different types since any_cast to Object is prone to failure
                environment.get()->define(dynamic_cast<Function*>(declaration.get())->params.at(i).lexeme, arguments.at(i));
            }

            try {
                interpreter.executeBlock(dynamic_cast<Function*>(declaration.get())->body, environment);
            } catch(const ReturnError& err) {
                return err.value;
            }

            return std::nullptr_t{};
        }
};

#endif