#include "Expr.hpp"

class Expr::Binary : Expr {
    Binary(Expr left, Token operator_, Expr right) {
      this->left = left;
      this->operator_ = operator_;
      this->right = right;
    };

    std::any accept(Visitor visitor) override {
      return visitor.visitBinaryExpr(*this);
    }
    Expr left;
    Token operator_;
    Expr right;
};
class Expr::Grouping : Expr {
    Grouping(Expr expression) {
      this->expression = expression;
    };

    std::any accept(Visitor visitor) override {
      return visitor.visitGroupingExpr(*this);
    }
    Expr expression;
};
class Expr::Literal : Expr {
    Literal(Object value) {
      this->value = value;
    };

    std::any accept(Visitor visitor) override {
      return visitor.visitLiteralExpr(*this);
    }
    Object value;
};
class Expr::Unary : Expr {
    Unary(Token operator_, Expr right) {
      this->operator_ = operator_;
      this->right = right;
    };

    std::any accept(Visitor visitor) override {
      return visitor.visitUnaryExpr(*this);
    }
    Token operator_;
    Expr right;
};
