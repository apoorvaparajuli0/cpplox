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

