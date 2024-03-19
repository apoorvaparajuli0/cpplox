#include "Expr.hpp"

class AstPrinter : Expr::Visitor {
    std::string print(Expr expr) {
        return std::any_cast<std::string>(expr.accept(*this));
    }

    public:
        std::any visitBinaryExpr(Expr::Binary expr) override {
            return parenthesize(expr.operator.lexeme, expr.left, expr.right);
        }
};