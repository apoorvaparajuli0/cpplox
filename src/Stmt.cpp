#include "../headers/Stmt.hpp"

Block::Block(std::vector<stmt_ptr> statements) : 
statements{std::move(statements)} {}
void Block::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitBlockStmt(*this);
}

Expression::Expression(expr_ptr expression) : 
expression{std::move(expression)} {}
void Expression::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitExpressionStmt(*this);
}

Function::Function(Token name, std::vector<Token> params, std::vector<stmt_ptr> body) : 
name{std::move(name)}, params{std::move(params)}, body{std::move(body)} {}
void Function::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitFunctionStmt(*this);
}

If::If(expr_ptr condition, stmt_ptr thenBranch, stmt_ptr elseBranch) : 
condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)} {}
void If::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitIfStmt(*this);
}

Print::Print(expr_ptr expression) : 
expression{std::move(expression)} {}
void Print::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitPrintStmt(*this);
}

Var::Var(Token name, expr_ptr initializer) : 
name{std::move(name)}, initializer{std::move(initializer)} {}
void Var::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitVarStmt(*this);
}

While::While(expr_ptr condition, stmt_ptr body) : 
condition{std::move(condition)}, body{std::move(body)} {}
void While::accept(StmtVisitor<void>& visitor) const {
  return visitor.visitWhileStmt(*this);
}

