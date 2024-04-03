#ifndef LOXLAMBDA_HPP
#define LOXLAMBDA_HPP

//CHALLENGE 10.2: Add Support for Lambda Expressions
// #include "LoxCallable.hpp"
// #include "Visitor.hpp"
// #include "Expr.hpp"
// #include "Environment.hpp"
// #include "Interpreter.hpp"
// #include "ReturnError.hpp"

// class LoxLambda : public LoxCallable {
//     private:
//         expr_ptr declaration;
//         env_ptr closure;

//     public:
//         LoxLambda(const Lambda& expression, env_ptr& closure) {
//             this->declaration = expr_ptr(new Lambda(expression));
//             this->closure = closure;
//         }

//         std::string toString() override {
//             return "<fn lambda >";
//         }

//         int arity() override {
//             return dynamic_cast<Lambda*>(declaration.get())->params.size();
//         }

//         Object call(Interpreter& interpreter, std::vector<Object> arguments) override {
//             env_ptr environment = env_ptr(new Environment(closure));

//             for (int i = 0; i < dynamic_cast<Lambda*>(declaration.get())->params.size(); i++) {
//                 environment.get()->define(dynamic_cast<Lambda*>(declaration.get())->params.at(i).lexeme, arguments.at(i));
//             }

//             try {
//                 interpreter.executeBlock(dynamic_cast<Lambda*>(declaration.get())->body, environment);
//             } catch(const ReturnError& err) {
//                 return err.value;
//             }

//             return std::nullptr_t{};
//         }
// };

#endif