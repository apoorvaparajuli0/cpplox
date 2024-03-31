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
class Function : public Stmt {
  public:
    Token name;
    std::vector<Token> params;
    std::vector<stmt_ptr> body;

  Function(Token name, std::vector<Token> params, std::vector<stmt_ptr> body);
  void accept(StmtVisitor<void>& visitor) const override;
};
class If : public Stmt {
  public:
    expr_ptr condition;
    stmt_ptr thenBranch;
    stmt_ptr elseBranch;

  If(expr_ptr condition, stmt_ptr thenBranch, stmt_ptr elseBranch);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Print : public Stmt {
  public:
    expr_ptr expression;

  Print(expr_ptr expression);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Return : public Stmt {
  public:
    Token keyword;
    expr_ptr value;

  Return(Token keyword, expr_ptr value);
  void accept(StmtVisitor<void>& visitor) const override;
};
class Var : public Stmt {
  public:
    Token name;
    expr_ptr initializer;

  Var(Token name, expr_ptr initializer);
  void accept(StmtVisitor<void>& visitor) const override;
};
class While : public Stmt {
  public:
    expr_ptr condition;
    stmt_ptr body;

  While(expr_ptr condition, stmt_ptr body);
  void accept(StmtVisitor<void>& visitor) const override;
};

#endif
