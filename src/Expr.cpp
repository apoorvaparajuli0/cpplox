#include "../headers/Expr.hpp"

Binary::Binary(expr_ptr left, Token operator_, expr_ptr right) : 
left{std::move(left)}, operator_{std::move(operator_)}, right{std::move(right)} {}
Object Binary::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitBinaryExpr(*this);
}

Grouping::Grouping(expr_ptr expression) : 
expression{std::move(expression)} {}
Object Grouping::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitGroupingExpr(*this);
}

Literal::Literal(Object value) : 
value{std::move(value)} {}
Object Literal::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitLiteralExpr(*this);
}

Unary::Unary(Token operator_, expr_ptr right) : 
operator_{std::move(operator_)}, right{std::move(right)} {}
Object Unary::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitUnaryExpr(*this);
}

Variable::Variable(Token name) : 
name{std::move(name)} {}
Object Variable::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitVariableExpr(*this);
}

