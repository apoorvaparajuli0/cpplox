#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include "string"
#include "initializer_list"
#include "sstream"

#include "Visitor.hpp"
#include "LoxCallable.hpp"

class AstPrinter : ExprVisitor<Object> {
    public:
        std::string print(std::initializer_list<expr_ptr>& exprs);
        std::string parenthesize(std::string name, std::initializer_list<const Expr*> exprs);
        Object visitBinaryExpr(const Binary& expr) override;
        Object visitGroupingExpr(const Grouping& expr) override;
        Object visitLiteralExpr(const Literal& expr) override;
        Object visitUnaryExpr(const Unary& expr) override;

    private:
        std::stringstream oss;
};

#endif