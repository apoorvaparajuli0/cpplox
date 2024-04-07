#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "vector"
#include "chrono"

#include "Visitor.hpp"
#include "Environment.hpp"

class Interpreter : ExprVisitor<Object>, StmtVisitor<void> {

    public:
        std::unordered_map<const Expr*, int> locals;
        env_ptr globals = env_ptr(new Environment());
        env_ptr environment = globals;

        Interpreter();
        void interpret(std::vector<stmt_ptr>& statements);
        void resolve(const Expr& expr, int depth);

        Object visitBinaryExpr(const Binary& expr) override;
        Object visitCallExpr(const Call& expr) override;
        Object visitGetExpr(const Get& expr) override;

        //CHALLENGE 10.2: Add Support for Lambda Expressions
        // Object visitLambdaExpr(const Lambda& expr) override;
        
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitLogicalExpr(const Logical& expr) override;
        Object visitSetExpr(const Set& expr) override;
        Object visitThisExpr(const This& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;
        Object visitVariableExpr(const Variable& expr) override;
        Object visitAssignExpr(const Assign& expr) override;

        //CHALLENGE 9.3: Add Support for Break Statements
        // void visitBreakStmt(const Break& stmt) override;
        
        void visitExpressionStmt(const Expression& stmt) override;
        void visitFunctionStmt(const Function& stmt) override;
        void visitIfStmt(const If& stmt) override;
        void visitPrintStmt(const Print& stmt) override;
        void visitReturnStmt(const Return& stmt) override;
        void visitVarStmt(const Var& stmt) override;
        void visitWhileStmt(const While& stmt) override;
        void visitBlockStmt(const Block& stmt) override;
        void visitClassStmt(const Class& stmt) override;
        void executeBlock(const std::vector<stmt_ptr>& statements, const env_ptr& environment);

    private:
        class Clock : public LoxCallable {
            public:
                int arity() override {
                    return 0;
                }

                Object call(Interpreter& interpreter, std::vector<Object>& arguments) override {  
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

        Object lookupVariable(Token name, const Expr* expr);
        void lookupAssignment(Token name, const Expr* expr, const Object& value);
        bool isTruthy(const Object& object);
        bool isEqual(const Object& a, const Object& b);
        std::string stringify(const Object& object);
        void checkNumberOperand(Token op, const Object& operand);
        void checkNumberOperands(Token op, const Object& left, const Object& right);
};

#endif