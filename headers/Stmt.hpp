#ifndef STMT_HPP
#define STMT_HPP

#include "../headers/Visitor.hpp"
#include "vector"

class Block : public Stmt {
  public:
    std::vector<stmt_ptr> statements;

  Block(std::vector<stmt_ptr> statements);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Expression : public Stmt {
  public:
    expr_ptr expression;

  Expression(expr_ptr expression);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Print : public Stmt {
  public:
    expr_ptr expression;

  Print(expr_ptr expression);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Var : public Stmt {
  public:
    Token name;
    expr_ptr initializer;

  Var(Token name, expr_ptr initializer);
  void accept(StmtVisitor<void>& visitor) const override;
};

#endif
