#ifndef EXPR_HPP
#define EXPR_HPP

#include "../headers/Visitor.hpp"

class Assign : public Expr {
  public:
    Token name;
    expr_ptr value;

  Assign(Token name, expr_ptr value);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Binary : public Expr {
  public:
    expr_ptr left;
    Token operator_;
    expr_ptr right;

  Binary(expr_ptr left, Token operator_, expr_ptr right);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Grouping : public Expr {
  public:
    expr_ptr expression;

  Grouping(expr_ptr expression);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Literal : public Expr {
  public:
    Object value;

  Literal(Object value);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Unary : public Expr {
  public:
    Token operator_;
    expr_ptr right;

  Unary(Token operator_, expr_ptr right);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Variable : public Expr {
  public:
    Token name;

  Variable(Token name);
  Object accept(ExprVisitor<Object>& visitor) const override;
};

#endif
