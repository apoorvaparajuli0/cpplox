#include "../headers/Interpreter.hpp"
#include "../headers/RuntimeError.hpp"
#include "../headers/Lox.hpp"
#include "../headers/Token.hpp"
#include "memory"
#include "utility"

void Interpreter::interpret(std::vector<stmt_ptr> statements) {
    try {
        for(const stmt_ptr& statement : statements) {
            execute(statement);
        }
    } catch(RuntimeError& err) {
        Lox::runtimeError(err);
    }
}

Object Interpreter::visitGroupingExpr(const Grouping& expr) {
    return evaluate(expr.expression);
}

Object Interpreter::visitLiteralExpr(const Literal& expr) {
    return expr.value;
}

Object Interpreter::visitUnaryExpr(const Unary& expr) {
    Object right = evaluate(expr.right);

    switch(expr.operator_.type) {
        case MINUS:
            checkNumberOperand(expr.operator_, right);
            return -(std::any_cast<double>(std::visit(Token::ValueResolver{}, right)));
        case BANG:
            return !isTruthy(right);
    }

    return std::nullptr_t{};
}

Object Interpreter::visitVariableExpr(const Variable& expr) {
    return environment.get()->get(expr.name);
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
        return std::any_cast<bool>(std::visit(Token::ValueResolver{}, object)) == true ? "true" : "false";
    }

    return std::any_cast<std::string>(std::visit(Token::ValueResolver{}, object));
}

Object Interpreter::evaluate(const expr_ptr& expr) {
    return expr.get()->accept(*this);
}

void Interpreter::execute(const stmt_ptr& stmt) {
    stmt.get()->accept(*this);
}

void Interpreter::executeBlock(const std::vector<stmt_ptr>& statements, const env_ptr& environment) {
    env_ptr previous = this->environment;
    try {
        this->environment = environment;

        for(const stmt_ptr& statement : statements) {
            execute(statement);
        }

    } catch(...) {}

    this->environment = previous;
}

std::any Interpreter::visitBlockStmt(const Block& stmt) {
    executeBlock(std::move(stmt.statements), env_ptr(new Environment(environment)));

    return NULL;
}
std::any Interpreter::visitExpressionStmt(const Expression &stmt)
{
    evaluate(stmt.expression);
    return NULL;
}

std::any Interpreter::visitPrintStmt(const Print& stmt) {
    Object value = evaluate(stmt.expression);
    printf("%s\n", stringify(value).c_str());

    return NULL;
}

std::any Interpreter::visitVarStmt(const Var& stmt) {
    Object value = std::nullptr_t{};
    if(stmt.initializer != std::nullptr_t{}) {
        value = evaluate(stmt.initializer);
    }

    environment.get()->define(stmt.name.lexeme, value);
    return NULL;
}

Object Interpreter::visitAssignExpr(const Assign& expr) {
    Object value = evaluate(expr.value);
    environment.get()->assign(expr.name, value);

    return value;
}

Object Interpreter::visitBinaryExpr(const Binary& expr) {
    Object left = evaluate(expr.left);
    Object right = evaluate(expr.right);

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
            } 
            //CHALLENGE 7.2: When either operand of '+' is string, convert other operand to string and concatenate
            // else if(std::holds_alternative<std::string>(left) != std::holds_alternative<std::string>(right)) {
            //     return stringify(left) + stringify(right);
            // } 
            else if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::any_cast<std::string>(left_operand) + std::any_cast<std::string>(right_operand);
            } else {
                throw RuntimeError(expr.operator_, "Operands Must be Two Numbers or Two Strings");
            }
            break;
        case SLASH:
            checkNumberOperands(expr.operator_, left, right);
            //CHALLENGE 7.3: Throw error for division by zero
            // if(std::any_cast<double>(right) == 0) throw RuntimeError(expr.operator_, "Attempted Division By Zero");
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case STAR:
            checkNumberOperands(expr.operator_, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
    }

    return std::nullptr_t{};
}