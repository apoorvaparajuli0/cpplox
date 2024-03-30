#include "../headers/Expr.hpp"

Assign::Assign(Token name, expr_ptr value) : 
name{std::move(name)}, value{std::move(value)} {}
Object Assign::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitAssignExpr(*this);
}

Binary::Binary(expr_ptr left, Token operator_, expr_ptr right) : 
left{std::move(left)}, operator_{std::move(operator_)}, right{std::move(right)} {}
Object Binary::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitBinaryExpr(*this);
}

Call::Call(expr_ptr callee, Token paren, std::vector<expr_ptr> arguments) : 
callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)} {}
Object Call::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitCallExpr(*this);
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

Logical::Logical(expr_ptr left, Token operator_, expr_ptr right) : 
left{std::move(left)}, operator_{std::move(operator_)}, right{std::move(right)} {}
Object Logical::accept(ExprVisitor<Object>& visitor) const {
  return visitor.visitLogicalExpr(*this);
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

