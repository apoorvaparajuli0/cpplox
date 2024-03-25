#include "../headers/Interpreter.hpp"
#include "../headers/RuntimeError.hpp"
#include "../headers/Lox.hpp"
#include "utility"

void Interpreter::interpret(expr_ptr expression) {
    try {
        Object value = std::any_cast<Object>(evaluate(expression));
        printf("%s\n", stringify(value).c_str());
    } catch(RuntimeError& err) {
        Lox::runtimeError(err);
    }
}

std::any Interpreter::visitGroupingExpr(const Grouping& expr) {
    return evaluate(expr.expression);
}

std::any Interpreter::visitLiteralExpr(const Literal& expr) {
    return expr.value;
}

std::any Interpreter::visitUnaryExpr(const Unary& expr) {
    Object right = std::any_cast<Object>(evaluate(expr.right));

    switch(expr.operator_.type) {
        case MINUS:
            checkNumberOperand(expr.operator_, right);
            return -(std::any_cast<double>(std::visit(Token::ValueResolver{}, right)));
        case BANG:
            return !isTruthy(right);
    }

    return NULL;
}

void Interpreter::checkNumberOperand(Token op, Object operand) {
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand Must be a Number");
}

void Interpreter::checkNumberOperands(Token op, Object left, Object right) {
    if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands Must be Numbers");
}

bool Interpreter::isTruthy(Object object) {
    if(std::holds_alternative<std::nullptr_t>(object)) return false;
    if(std::holds_alternative<bool>(object)) return std::any_cast<bool>(std::visit(Token::ValueResolver{}, object));
    return true;
}

bool Interpreter::isEqual(Object a, Object b) {
    if((std::holds_alternative<std::nullptr_t>(a) && std::holds_alternative<std::nullptr_t>(b))) return true;
    if((std::holds_alternative<std::nullptr_t>(a))) return false;
    if(std::visit(Token::TypeResolver{}, a) != std::visit(Token::TypeResolver{}, b)) return false;

    auto equality = [](Object& op1, Object& op2) -> bool {
        if(const double* ptr = std::get_if<double>(&op1)) {
            return *ptr == *(std::get_if<double>(&op2));
        } else if(const bool* ptr = std::get_if<bool>(&op1)) {
            return *ptr == *(std::get_if<bool>(&op2));
        } else if(const std::string* ptr = std::get_if<std::string>(&op1)) {
            return *ptr == *(std::get_if<std::string>(&op2));
        }
        
        return false;
    };

    return equality(a, b);
}

std::string Interpreter::stringify(Object object) {
    if(std::holds_alternative<std::nullptr_t>(object)) return "nil";
    if(std::holds_alternative<double>(object)) {
        std::stringstream oss;
        oss << std::any_cast<double>(std::visit(Token::ValueResolver{}, object));
        if(oss.str().find(".0") != std::string::npos) {
            return oss.str().substr(0, oss.str().find(".0"));
        }
        return oss.str();
    }

    if(std::holds_alternative<bool>(object)) {
        return std::to_string(std::any_cast<bool>(std::visit(Token::ValueResolver{}, object)));
    }

    return std::any_cast<std::string>(std::visit(Token::ValueResolver{}, object));
}

std::any Interpreter::evaluate(const expr_ptr& expr) {
    return expr.get()->accept(*this);
}

std::any Interpreter::visitBinaryExpr(const Binary& expr) {
    Object left = std::any_cast<Object>(evaluate(expr.left));
    Object right = std::any_cast<Object>(evaluate(expr.right));

    std::any left_operand = std::visit(Token::ValueResolver{}, left);
    std::any right_operand = std::visit(Token::ValueResolver{}, right);

    switch(expr.operator_.type) {
        case GREATER:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left_operand) > std::any_cast<double>(right_operand);
        case GREATER_EQUAL:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left_operand) >= std::any_cast<double>(right_operand);
        case LESS:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left_operand) < std::any_cast<double>(right_operand);
        case LESS_EQUAL:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left_operand) <= std::any_cast<double>(right_operand);

        case BANG_EQUAL: return !isEqual(left, right);
        case EQUAL_EQUAL: return isEqual(left, right);

        case MINUS:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left_operand) - std::any_cast<double>(right_operand);
        case PLUS:
            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::any_cast<double>(left_operand) + std::any_cast<double>(right_operand);
            } else if(std::holds_alternative<std::string>(left) != std::holds_alternative<std::string>(right)) {
                return stringify(left) + stringify(right);
            } else if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::any_cast<std::string>(left_operand) + std::any_cast<std::string>(right_operand);
            } else {
                throw RuntimeError(expr.operator_, "Operands Must be Two Numbers or Two Strings");
            }
            break;
        case SLASH:
            checkNumberOperands(expr.operator_, left, right);
            if(std::any_cast<double>(right) == 0) throw RuntimeError(expr.operator_, "Attempted Division By Zero");
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case STAR:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
    }

    return NULL;
}