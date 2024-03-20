#include "Expr.hpp"

    Binary::Binary(expr_ptr left, Token operator_, expr_ptr right) : 
    left{std::move(left)}, operator_{std::move(operator_)}, right{std::move(right)} {}
    std::any Binary::accept(Visitor<std::any>& visitor) const {
      return visitor.visitBinaryExpr(*this);
    }

    Grouping::Grouping(expr_ptr expression) : 
    expression{std::move(expression)} {}
    std::any Grouping::accept(Visitor<std::any>& visitor) const {
      return visitor.visitGroupingExpr(*this);
    }

    Literal::Literal(Object value) : 
    value{std::move(value)} {}
    std::any Literal::accept(Visitor<std::any>& visitor) const {
      return visitor.visitLiteralExpr(*this);
    }

    Unary::Unary(Token operator_, expr_ptr right) : 
    operator_{std::move(operator_)}, right{std::move(right)} {}
    std::any Unary::accept(Visitor<std::any>& visitor) const {
      return visitor.visitUnaryExpr(*this);
    }

