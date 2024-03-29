#include "../headers/RPNPrinter.hpp"

//CHALLENGE 5.3: Create a Reverse Polish Notation Printer

std::string RPNPrinter::print(std::initializer_list<expr_ptr>& exprs) {

    for(const auto& expr : exprs) {
        expr.get()->accept(*this);
    }

    return oss.str();
}

std::string RPNPrinter::format(std::string name, std::initializer_list<const Expr*> exprs) {

    for(const auto& expr : exprs) {
        oss << " ";
        oss << std::any_cast<std::string>(expr->accept(*this));
        oss << " ";
    }
    oss << name;

    return {};
}

Object RPNPrinter::visitBinaryExpr(const Binary& expr) {
    return format(expr.operator_.lexeme, {std::move(expr.left.get()), std::move(expr.right.get())});
}

Object RPNPrinter::visitGroupingExpr(const Grouping& expr) {
    return format("", {std::move(expr.expression.get())});
}

Object RPNPrinter::visitLiteralExpr(const Literal& expr) {
    if(std::visit(Token::Resolver{}, expr.value) == "null") return "nil";
    oss << std::visit(Token::Resolver{}, expr.value);

    return std::string{""};
}

Object RPNPrinter::visitUnaryExpr(const Unary& expr) {
    return format(expr.operator_.lexeme, {std::move(expr.right.get())});
}
