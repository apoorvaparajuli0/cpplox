#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "vector"
#include "chrono"

#include "Visitor.hpp"
#include "Environment.hpp"

class Interpreter : ExprVisitor<Object>, StmtVisitor<void> {

    public:
        env_ptr globals = env_ptr(new Environment());
        env_ptr environment = globals;

        Interpreter();
        void interpret(std::vector<stmt_ptr> statements);

        Object visitBinaryExpr(const Binary& expr) override;
        Object visitCallExpr(const Call& expr) override;
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitLogicalExpr(const Logical& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;
        Object visitVariableExpr(const Variable& expr) override;
        Object visitAssignExpr(const Assign& expr) override;

        //CHALLENGE 9.3: Add Support for Break Statements
        // void visitBreakStmt(const Break& stmt) override;
        
        void visitExpressionStmt(const Expression& stmt) override;
        void visitFunctionStmt(const Function& stmt) override;
        void visitIfStmt(const If& stmt) override;
        void visitPrintStmt(const Print& stmt) override;
        void visitVarStmt(const Var& stmt) override;
        void visitWhileStmt(const While& stmt) override;
        void visitBlockStmt(const Block& stmt) override;
        void executeBlock(const std::vector<stmt_ptr>& statements, const env_ptr& environment);

    private:
        class Clock : public LoxCallable {
            public:
                int arity() override {
                    return 0;
                }

                Object call(Interpreter& interpreter, std::vector<Object> arguments) override {  
                    return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
                }

                std::string toString() override {
                    return "<native fn>";
                }
        };

        //CHALLENGE 9.3: Add Support for Break Statements
        //kind of an inelegant solution to the "break" problem, but
        //wanted to just finish up chapter
        // bool breakEncountered = false;

        Object evaluate(const expr_ptr& expr);
        void execute(const stmt_ptr& stmt);

        bool isTruthy(Object object);
        bool isEqual(Object a, Object b);
        std::string stringify(Object object);
        void checkNumberOperand(Token op, Object operand);
        void checkNumberOperands(Token op, Object left, Object right);
};

#endif