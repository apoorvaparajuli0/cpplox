#ifndef EXPR_HPP
#define EXPR_HPP

#include "../headers/Visitor.hpp"
#include "vector"

class Lambda : public Expr {
  public:
    std::vector<Token> params;
    std::vector<stmt_ptr> body;

  Lambda(std::vector<Token> params, std::vector<stmt_ptr> body);
  Object accept(ExprVisitor<Object>& visitor) const override;
};
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
