#include "../headers/Visitor.hpp"

class Binary : public Expr {
  public:
    expr_ptr left;
    Token operator_;
    expr_ptr right;

  Binary(expr_ptr left, Token operator_, expr_ptr right);
  std::any accept(Visitor<std::any>& visitor) const override;
};
class Grouping : public Expr {
  public:
    expr_ptr expression;

  Grouping(expr_ptr expression);
  std::any accept(Visitor<std::any>& visitor) const override;
};
class Literal : public Expr {
  public:
    Object value;

  Literal(Object value);
  std::any accept(Visitor<std::any>& visitor) const override;
};
class Unary : public Expr {
  public:
    Token operator_;
    expr_ptr right;

  Unary(Token operator_, expr_ptr right);
  std::any accept(Visitor<std::any>& visitor) const override;
};
