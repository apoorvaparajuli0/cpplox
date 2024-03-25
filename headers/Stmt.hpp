#include "../headers/Visitor.hpp"

class Expression : public Stmt {
  public:
    Expr expression;

  Expression(Expr expression);
  std::any accept(Visitor<std::any>& visitor) const override;
};
class Print : public Stmt {
  public:
    Expr expression;

  Print(Expr expression);
  std::any accept(Visitor<std::any>& visitor) const override;
};
