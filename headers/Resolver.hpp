#ifndef RESOLVER_HPP
#define RESOLVER_HPP

#include "stack"

#include "Visitor.hpp"
#include "Interpreter.hpp"

class Resolver : ExprVisitor<Object>, StmtVisitor<void> {

    class ResolverStack : public std::stack<std::unordered_map<std::string, bool>> {
        public:
            std::unordered_map<std::string, bool>& at(int index) {
                return c[index];
            }
    };

    private:
        enum class FunctionType {
            NONE,
            FUNCTION,
            INITIALIZER,
            METHOD
        };

        enum class ClassType {
            NONE,
            CLASS,
            SUBCLASS
        };

        Interpreter& interpreter;
        ResolverStack scopes;
        FunctionType currentFunction = FunctionType::NONE;
        ClassType currentClass = ClassType::NONE;

        void resolve(const stmt_ptr& stmt);
        void resolve(const expr_ptr& expr);
        void resolveFunction(const Function* stmt, FunctionType function);
        void beginScope();
        void endScope();
        void declare(Token name);
        void define(Token name);
        void resolveLocal(const Expr* expr, Token name);

    public:
        Resolver(Interpreter& interpreter);

        Object visitBinaryExpr(const Binary& expr) override;
        Object visitCallExpr(const Call& expr) override;
        Object visitGetExpr(const Get& expr) override;
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitLogicalExpr(const Logical& expr) override;
        Object visitSetExpr(const Set& expr) override;
        Object visitSuperExpr(const Super& expr) override;
        Object visitThisExpr(const This& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;
        Object visitVariableExpr(const Variable& expr) override;
        Object visitAssignExpr(const Assign& expr) override;

        void visitExpressionStmt(const Expression& stmt) override;
        void visitFunctionStmt(const Function& stmt) override;
        void visitIfStmt(const If& stmt) override;
        void visitPrintStmt(const Print& stmt) override;
        void visitReturnStmt(const Return& stmt) override;
        void visitVarStmt(const Var& stmt) override;
        void visitWhileStmt(const While& stmt) override;
        void visitBlockStmt(const Block& stmt) override;
        void visitClassStmt(const Class& stmt) override;

        void resolve(const std::vector<stmt_ptr>& statements);
};

#endif