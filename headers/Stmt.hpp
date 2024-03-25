#ifndef STMT_HPP
#define STMT_HPP

#include "../headers/Visitor.hpp"

class Expression : public Stmt {
  public:
    expr_ptr expression;

  Expression(expr_ptr expression);
  std::any accept(StmtVisitor<std::any>& visitor) const override;
};
class Print : public Stmt {
  public:
    expr_ptr expression;

  Print(expr_ptr expression);
  std::any accept(StmtVisitor<std::any>& visitor) const override;
};

#endif
