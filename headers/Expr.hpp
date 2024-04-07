#ifndef EXPR_HPP
#define EXPR_HPP

#include "../headers/Visitor.hpp"
#include "vector"

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
class Call : public Expr {
  public:
    expr_ptr callee;
    Token paren;
    std::vector<expr_ptr> arguments;

  Call(expr_ptr callee, Token paren, std::vector<expr_ptr> arguments);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Get : public Expr {
  public:
    expr_ptr object;
    Token name;

  Get(expr_ptr object, Token name);
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
class Logical : public Expr {
  public:
    expr_ptr left;
    Token operator_;
    expr_ptr right;

  Logical(expr_ptr left, Token operator_, expr_ptr right);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class Set : public Expr {
  public:
    expr_ptr object;
    Token name;
    expr_ptr value;

  Set(expr_ptr object, Token name, expr_ptr value);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
class This : public Expr {
  public:
    Token keyword;

  This(Token keyword);
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
