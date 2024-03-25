#ifndef RPNPRINTER_HPP
#define RPNPRINTER_HPP

//CHALLENGE 5.3: Create a Reverse Polish Notation Printer

#include "initializer_list"
#include "list"
#include "Expr.hpp"

class RPNPrinter : ExprVisitor<std::any> {
    public:
        std::string print(std::initializer_list<expr_ptr>& exprs);
        std::string format(std::string name, std::initializer_list<const Expr*> exprs);
        std::any visitBinaryExpr(const Binary& expr) override;
        std::any visitGroupingExpr(const Grouping& expr) override;
        std::any visitLiteralExpr(const Literal& expr) override;
        std::any visitUnaryExpr(const Unary& expr) override;

    private:
        std::stringstream oss;
};

#endif