#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../headers/Visitor.hpp"
#include "Expr.hpp"

class Interpreter : Visitor<std::any> {

    public:
        void interpret(expr_ptr expr);

        std::any visitBinaryExpr(const Binary& expr) override;
        std::any visitGroupingExpr(const Grouping& expr) override;
        std::any visitLiteralExpr(const Literal& expr) override;
        std::any visitUnaryExpr(const Unary& expr) override;

    private:
        std::any evaluate(const expr_ptr& expr);
        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        std::string stringify(Object object);
        void checkNumberOperand(Token op, Object operand);
        void checkNumberOperands(Token op, Object left, Object right);
};

#endif