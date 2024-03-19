#include "string"
#include "Expr.hpp"
#include "Visitor.hpp"

class AstPrinter : Visitor {
    std::string print(Expr expr) {
        return std::any_cast<std::string>(expr.accept(*this));
    }

    std::string parenthesize(std::string lexeme, Expr left, Expr right) {

    }

    std::any visitBinaryExpr(Expr::Binary expr) override {
        return parenthesize(expr.operator_.lexeme, expr.left, expr.right);
    }
};