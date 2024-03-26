#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../headers/Visitor.hpp"
#include "../headers/Environment.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "vector"

class Interpreter : ExprVisitor<Object>, StmtVisitor<std::any> {

    public:
        void interpret(std::vector<stmt_ptr> statements);

        Object visitBinaryExpr(const Binary& expr) override;
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;
        Object visitVariableExpr(const Variable& expr) override;

        std::any visitExpressionStmt(const Expression& stmt) override;
        std::any visitPrintStmt(const Print& stmt) override;
        std::any visitVarStmt(const Var& stmt) override;

    private:
        Environment environment = Environment();

        Object evaluate(const expr_ptr& expr);
        void execute(const stmt_ptr& stmt);
        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        std::string stringify(Object object);
        void checkNumberOperand(Token op, Object operand);
        void checkNumberOperands(Token op, Object left, Object right);
};

#endif