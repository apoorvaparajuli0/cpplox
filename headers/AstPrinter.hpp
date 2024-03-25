#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include "initializer_list"
#include "list"
#include "Expr.hpp"

class AstPrinter : ExprVisitor<std::any> {
    public:
        std::string print(std::initializer_list<expr_ptr>& exprs);
        std::string parenthesize(std::string name, std::initializer_list<const Expr*> exprs);
        std::any visitBinaryExpr(const Binary& expr) override;
        std::any visitGroupingExpr(const Grouping& expr) override;
        std::any visitLiteralExpr(const Literal& expr) override;
        std::any visitUnaryExpr(const Unary& expr) override;

    private:
        std::stringstream oss;
};

#endif