#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../headers/Visitor.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "vector"

class Interpreter : ExprVisitor<std::any>, StmtVisitor<std::any> {

    public:
        void interpret(std::vector<stmt_ptr> statements);

        std::any visitBinaryExpr(const Binary& expr) override;
        std::any visitGroupingExpr(const Grouping& expr) override;
        std::any visitLiteralExpr(const Literal& expr) override;
        std::any visitUnaryExpr(const Unary& expr) override;

        std::any visitExpressionStmt(const Expression& stmt) override;
        std::any visitPrintStmt(const Print& stmt) override;

    private:
        std::any evaluate(const expr_ptr& expr);
        void execute(const stmt_ptr& stmt);
        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        std::string stringify(Object object);
        void checkNumberOperand(Token op, Object operand);
        void checkNumberOperands(Token op, Object left, Object right);
};

#endif