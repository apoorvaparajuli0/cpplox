#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "any"
#include "memory"
#include "../headers/Token.hpp"

class Assign;
class Binary;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;


class Block;
class Expression;
class If;
class Print;
class Var;
class While;


template<class R>
class ExprVisitor {
  public:
    virtual R visitAssignExpr(const Assign& expr) = 0;
    virtual R visitBinaryExpr(const Binary& expr) = 0;
    virtual R visitGroupingExpr(const Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Literal& expr) = 0;
    virtual R visitLogicalExpr(const Logical& expr) = 0;
    virtual R visitUnaryExpr(const Unary& expr) = 0;
    virtual R visitVariableExpr(const Variable& expr) = 0;
    virtual ~ExprVisitor() = default;
};

template<class R>
class StmtVisitor {
  public:
    virtual R visitBlockStmt(const Block& stmt) = 0;
    virtual R visitExpressionStmt(const Expression& stmt) = 0;
    virtual R visitIfStmt(const If& stmt) = 0;
    virtual R visitPrintStmt(const Print& stmt) = 0;
    virtual R visitVarStmt(const Var& stmt) = 0;
    virtual R visitWhileStmt(const While& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

class Expr {
  public:
    virtual Object accept(ExprVisitor<Object>& visitor) const = 0;
    virtual ~Expr() = default;
};

class Stmt {
  public:
    virtual void accept(StmtVisitor<void>& visitor) const = 0;
    virtual ~Stmt() = default;
};

using expr_ptr = std::unique_ptr<Expr>;
using stmt_ptr = std::unique_ptr<Stmt>;

#endif
