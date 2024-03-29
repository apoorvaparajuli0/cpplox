#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../headers/Visitor.hpp"
#include "../headers/Environment.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "vector"

class Interpreter : ExprVisitor<Object>, StmtVisitor<void> {

    public:

        void interpret(std::vector<stmt_ptr> statements);

        Object visitBinaryExpr(const Binary& expr) override;
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitLogicalExpr(const Logical& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;
        Object visitVariableExpr(const Variable& expr) override;
        Object visitAssignExpr(const Assign& expr) override;

        //CHALLENGE 9.3: Add Support for Break Statements
        // void visitBreakStmt(const Break& stmt) override;
        
        void visitExpressionStmt(const Expression& stmt) override;
        void visitIfStmt(const If& stmt) override;
        void visitPrintStmt(const Print& stmt) override;
        void visitVarStmt(const Var& stmt) override;
        void visitWhileStmt(const While& stmt) override;
        void visitBlockStmt(const Block& stmt) override;

    private:
        env_ptr environment = env_ptr(new Environment());

        //CHALLENGE 9.3: Add Support for Break Statements
        //kind of an inelegant solution to the "break" problem, but
        //wanted to just finish up chapter
        // bool breakEncountered = false;

        Object evaluate(const expr_ptr& expr);
        void execute(const stmt_ptr& stmt);
        void executeBlock(const std::vector<stmt_ptr>& statements, const env_ptr& environment);

        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        std::string stringify(Object object);
        void checkNumberOperand(Token op, Object operand);
        void checkNumberOperands(Token op, Object left, Object right);
};

#endif