#include "Expr.hpp"

 class Expr::Binary : Expr {
    Binary(Expr left, Token operator_, Expr right) {
      this->left = left;
      this->operator_ = operator_;
      this->right = right;
    };

    Expr left;
    Token operator_;
    Expr right;
  };
 class Expr::Grouping : Expr {
    Grouping(Expr expression) {
      this->expression = expression;
    };

    Expr expression;
  };
 class Expr::Literal : Expr {
    Literal(Object value) {
      this->value = value;
    };

    Object value;
  };
 class Expr::Unary : Expr {
    Unary(Token operator_, Expr right) {
      this->operator_ = operator_;
      this->right = right;
    };

    Token operator_;
    Expr right;
  };
