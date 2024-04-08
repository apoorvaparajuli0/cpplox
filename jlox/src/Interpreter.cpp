#include "../headers/Interpreter.hpp"
#include "../headers/RuntimeError.hpp"
#include "../headers/Expr.hpp"
#include "../headers/Stmt.hpp"
#include "../headers/LoxCallable.hpp"
#include "../headers/LoxClass.hpp"
#include "../headers/LoxFunction.hpp"
#include "../headers/ReturnError.hpp"
#include "../headers/Lox.hpp"

Interpreter::Interpreter() {
    Object value = call_ptr(new Clock());
    globals->define("clock", value);
}

void Interpreter::interpret(std::vector<stmt_ptr>& statements) {
    try {
        for(const stmt_ptr& statement : statements) {
            execute(statement);
        }
    } catch(const RuntimeError& err) {
        Lox::runtimeError(err);
    }
}

Object Interpreter::visitGroupingExpr(const Grouping& expr) {
    return evaluate(expr.expression);
}

Object Interpreter::visitLiteralExpr(const Literal& expr) {
    return expr.value;
}

Object Interpreter::visitLogicalExpr(const Logical& expr) {
    Object left = evaluate(expr.left);

    if(expr.operator_.type == TokenType::OR) {
        if(isTruthy(left)) return left;
    } else {
        if(!isTruthy(left)) return left;
    }

    return evaluate(expr.right);
}

Object Interpreter::visitSetExpr(const Set& expr) {
    Object object = evaluate(expr.object);

    if (!std::holds_alternative<instance_ptr>(object)) { 
      throw RuntimeError(expr.name, "Only instances have fields.");
    }

    Object value = evaluate(expr.value);
    dynamic_cast<LoxInstance*>(std::any_cast<instance_ptr>(std::visit(Token::ValueResolver{}, object)).get())->set(expr.name, value);
    
    return value;
}

Object Interpreter::visitSuperExpr(const Super& expr) {
    int distance = locals.at(&expr);
    call_ptr superclass = std::any_cast<call_ptr>(std::visit(Token::ValueResolver{}, environment->getAt(distance, "super")));
    instance_ptr object = std::any_cast<instance_ptr>(std::visit(Token::ValueResolver{}, environment->getAt(distance - 1, "this")));

    call_ptr method = dynamic_cast<LoxClass*>(superclass.get())->findMethod(expr.method.lexeme);

    if(method == std::nullptr_t{}) {
        throw RuntimeError(expr.method, "Undefined property '" + expr.method.lexeme + "'.");
    }

    return dynamic_cast<LoxFunction*>(method.get())->bind(*(object.get()));
}

Object Interpreter::visitThisExpr(const This& expr) {
    return lookupVariable(expr.keyword, &expr);
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
    return lookupVariable(expr.name, &expr);
}

Object Interpreter::lookupVariable(Token name, const Expr* expr) {
    int distance = -1;
    
    if(locals.contains(expr)) {
        distance = locals.at(expr);
    }

    if(distance != -1) {
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}

void Interpreter::checkNumberOperand(Token op, const Object& operand) {
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand Must be a Number");
}

void Interpreter::checkNumberOperands(Token op, const Object& left, const Object& right) {
    if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands Must be Numbers");
}

bool Interpreter::isTruthy(const Object& object) {
    if(std::holds_alternative<std::nullptr_t>(object)) return false;
    if(std::holds_alternative<bool>(object)) return std::any_cast<bool>(std::visit(Token::ValueResolver{}, object));
    return true;
}

bool Interpreter::isEqual(const Object& a, const Object& b) {
    if((std::holds_alternative<std::nullptr_t>(a) && std::holds_alternative<std::nullptr_t>(b))) return true;
    if((std::holds_alternative<std::nullptr_t>(a))) return false;
    if(std::visit(Token::TypeResolver{}, a) != std::visit(Token::TypeResolver{}, b)) return false;

    auto equality = [](const Object& op1, const Object& op2) -> bool {
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

std::string Interpreter::stringify(const Object& object) {
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
    return expr->accept(*this);
}

void Interpreter::execute(const stmt_ptr& stmt) {
    //CHALLENGE 8.1: Add Support for Expression Evaluation in REPL
    // if(dynamic_cast<Expression*>(stmt.get()) != nullptr && dynamic_cast<Expression*>(stmt.get())->pure) {
    //     Object result = evaluate(dynamic_cast<Expression*>(stmt.get())->expression);
    //     printf("%s\n", std::visit(Token::Resolver{}, result).c_str());
    // } else {
        stmt->accept(*this);
    // }
}

void Interpreter::resolve(const Expr& expr, int depth) {
    locals.try_emplace(&expr, depth);
}

void Interpreter::executeBlock(const std::vector<stmt_ptr>& statements, const env_ptr& environment) {
    env_ptr previous = this->environment;
    try {
        this->environment = environment;

        for(const stmt_ptr& statement : statements) {
            execute(statement);
        }

    } catch(const ReturnError& err) {
        this->environment = previous;
        throw err;
    } catch(const RuntimeError& err) {
        this->environment = previous;
        throw err;
    }

    this->environment = previous;
}

void Interpreter::visitBlockStmt(const Block& stmt) {
    executeBlock(std::move(stmt.statements), env_ptr(new Environment(environment)));
    return;
}

void Interpreter::visitClassStmt(const Class& stmt) {
    Object superclass = std::nullptr_t{};
    if(stmt.superclass != std::nullptr_t{}) {
        superclass = evaluate(stmt.superclass);

        if(!(std::holds_alternative<call_ptr>(superclass))) {
            throw RuntimeError(dynamic_cast<Variable*>(stmt.superclass.get())->name, "Superclass must be a class.");
        }
    }

    environment->define(stmt.name.lexeme, std::nullptr_t{});

    if(stmt.superclass != std::nullptr_t{}) {
        environment = env_ptr(new Environment(environment));
        environment->define("super", superclass);
    }

    std::unordered_map<std::string, call_ptr> methods;

    for(const stmt_ptr& method : stmt.methods) {
        Function* raw_method = dynamic_cast<Function*>(method.get());
        call_ptr function(new LoxFunction(method, environment, !(raw_method->name.lexeme.compare("init"))));
        methods[raw_method->name.lexeme] = function;
    }

    call_ptr klass;
    if(std::holds_alternative<call_ptr>(superclass)) {
        klass = call_ptr(new LoxClass(stmt.name.lexeme, std::any_cast<call_ptr>(std::visit(Token::ValueResolver{}, superclass)), methods));
    } else {
        klass = call_ptr(new LoxClass(stmt.name.lexeme, std::nullptr_t{}, methods));
    }

    if(!(std::holds_alternative<std::nullptr_t>(superclass))) {
        environment = environment->enclosing;
    }

    environment->assign(stmt.name, klass);

    return;
}

void Interpreter::visitExpressionStmt(const Expression& stmt)
{
    evaluate(stmt.expression);
    return;
}

void Interpreter::visitFunctionStmt(const Function& stmt) {
    call_ptr function(new LoxFunction(&stmt, environment, false));
    environment->define(stmt.name.lexeme, function);

    return;
}

//CHALLENGE 9.3: Add Support for Break Statements
// void Interpreter::visitBreakStmt(const Break& stmt) {
//     breakEncountered = true;
//     return;
// }

void Interpreter::visitIfStmt(const If& stmt) {
    if(isTruthy(evaluate(stmt.condition))) {
        execute(stmt.thenBranch);
    } else if(stmt.elseBranch != std::nullptr_t{}) {
        execute(stmt.elseBranch);
    }

    return;
}

void Interpreter::visitPrintStmt(const Print& stmt) {
    Object value = evaluate(stmt.expression);
    printf("%s\n", stringify(value).c_str());

    return;
}

void Interpreter::visitReturnStmt(const Return& stmt) {
    Object value = std::nullptr_t{};

    if(stmt.value != std::nullptr_t{}) value = evaluate(stmt.value);
    
    throw ReturnError(value);
}

void Interpreter::visitVarStmt(const Var& stmt) {
    Object value = std::nullptr_t{};
    if(stmt.initializer != std::nullptr_t{}) {
        value = evaluate(stmt.initializer);
    }

    environment->define(stmt.name.lexeme, value);

    return;
}

void Interpreter::visitWhileStmt(const While& stmt) {
    while(isTruthy(evaluate(stmt.condition))) {
        //CHALLENGE 9.3: Add Support for Break Statements
        // if(breakEncountered) {
        //     break;
        // }

        execute(stmt.body);
    }

    //CHALLENGE 9.3: Add Support for Break Statements
    // breakEncountered = false;

    return;
}

void Interpreter::lookupAssignment(Token name, const Expr* expr, const Object& value) {
    int distance = -1;
    
    if(locals.contains(expr)) {
        distance = locals.at(expr);
    }
    
    if(distance != -1) {
        environment->assignAt(distance, name, value);
    } else {
        globals->assign(name, value);
    }
}

Object Interpreter::visitAssignExpr(const Assign& expr) {
    Object value = evaluate(expr.value);
    lookupAssignment(expr.name, &expr, value);

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

Object Interpreter::visitCallExpr(const Call& expr) {
    Object callee = evaluate(expr.callee);

    std::vector<Object> arguments; 
    for (const expr_ptr& argument : expr.arguments) { 
      arguments.push_back(evaluate(argument));
    }

    if(!std::holds_alternative<call_ptr>(callee)) {
        throw RuntimeError(expr.paren, "Can only call functions and classes");
    }

    call_ptr function = std::any_cast<call_ptr>(std::visit(Token::ValueResolver{}, callee));

    if(arguments.size() != function->arity()) {
        throw RuntimeError(expr.paren, "Expected " + stringify((double)function->arity()) + " arguments but got " + stringify((double)arguments.size()) + ".");
    }

    return function->call(*this, arguments);
}

Object Interpreter::visitGetExpr(const Get& expr) {
    Object object = evaluate(expr.object);
    if (std::holds_alternative<instance_ptr>(object)) {
        return std::any_cast<instance_ptr>(std::visit(Token::ValueResolver{}, object))->get(expr.name);
    }

    throw RuntimeError(expr.name, "Only instances have properties.");
}

//CHALLENGE 10.2: Add Support for Lambda Expressions
// Object Interpreter::visitLambdaExpr(const Lambda& expr) {
//     call_ptr function(new LoxLambda(expr, environment));
//     return function;
// }