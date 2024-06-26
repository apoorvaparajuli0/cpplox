#include "../headers/Resolver.hpp"
#include "../headers/Lox.hpp"
#include "../headers/Stmt.hpp"
#include "../headers/Expr.hpp"

Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

void Resolver::resolve(const std::vector<stmt_ptr>& statements) {
    for (const stmt_ptr& statement : statements) {
        resolve(statement);
    }
}

void Resolver::visitBlockStmt(const Block& stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();

    return;
}

void Resolver::visitClassStmt(const Class& stmt) {
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;

    declare(stmt.name);
    define(stmt.name);

    if(stmt.superclass != std::nullptr_t{} && !(stmt.name.lexeme.compare(dynamic_cast<Variable*>(stmt.superclass.get())->name.lexeme))) {
        Lox::error(dynamic_cast<Variable*>(stmt.superclass.get())->name, "A class can't inherit from itself");
    }

    if(stmt.superclass != std::nullptr_t{}) {
        currentClass = ClassType::SUBCLASS;
        resolve(stmt.superclass);
    }

    if(stmt.superclass != std::nullptr_t{}) {
        beginScope();
        scopes.top()["super"] = true;
    }

    beginScope();
    scopes.top()["this"] = true;

    for(const stmt_ptr& method : stmt.methods) {
        FunctionType declaration = FunctionType::METHOD;
        if(!(dynamic_cast<Function*>(method.get())->name.lexeme.compare("init"))) {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFunction(dynamic_cast<Function*>(method.get()), declaration);
    }

    endScope();

    if(stmt.superclass != std::nullptr_t{}) endScope();

    currentClass = enclosingClass;

    return;
}

void Resolver::visitExpressionStmt(const Expression& stmt) {
    resolve(stmt.expression);
    return;
}

void Resolver::visitFunctionStmt(const Function& stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(&stmt, FunctionType::FUNCTION);

    return;
}

void Resolver::visitIfStmt(const If& stmt) {
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != std::nullptr_t{}) resolve(stmt.elseBranch);

    return;
}

void Resolver::visitPrintStmt(const Print& stmt) {
    resolve(stmt.expression);

    return;
}

void Resolver::visitReturnStmt(const Return& stmt) {
    if(currentFunction == FunctionType::NONE) {
        Lox::error(stmt.keyword, "Can't return from top-level code");
    }
    if (stmt.value != std::nullptr_t{}) {
      resolve(stmt.value);
    }
    if(currentFunction == FunctionType::INITIALIZER) {
        Lox::error(stmt.keyword, "Can't return a value from an initializer.");
    }

    return;
}

void Resolver::visitVarStmt(const Var& stmt) {
    declare(stmt.name);
    if (stmt.initializer != std::nullptr_t{}) {
      resolve(stmt.initializer);
    }
    define(stmt.name);

    return;
}

void Resolver::visitWhileStmt(const While& stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);

    return;
}

Object Resolver::visitAssignExpr(const Assign& expr) {
    resolve(expr.value);
    resolveLocal(&expr, expr.name);
    return std::nullptr_t{};
}

Object Resolver::visitBinaryExpr(const Binary& expr) {
    resolve(expr.left);
    resolve(expr.right);

    return std::nullptr_t{};
}

Object Resolver::visitCallExpr(const Call& expr) {
    resolve(expr.callee);

    for (const expr_ptr& argument : expr.arguments) {
      resolve(argument);
    }

    return std::nullptr_t{};
}

Object Resolver::visitGetExpr(const Get& expr) {
    resolve(expr.object);
    return std::nullptr_t{};
}

Object Resolver::visitGroupingExpr(const Grouping& expr) {
    resolve(expr.expression);

    return std::nullptr_t{};
}

Object Resolver::visitLiteralExpr(const Literal& expr) {
    return std::nullptr_t{};
}

Object Resolver::visitLogicalExpr(const Logical& expr) {
    resolve(expr.left);
    resolve(expr.right);

    return std::nullptr_t{};
}

Object Resolver::visitSetExpr(const Set& expr) {
    resolve(expr.value);
    resolve(expr.object);

    return std::nullptr_t{};
}

Object Resolver::visitSuperExpr(const Super& expr) {
    if(currentClass == ClassType::NONE) {
        Lox::error(expr.keyword, "Can't use 'super' outside of a class.");
    } else if(currentClass != ClassType::SUBCLASS) {
        Lox::error(expr.keyword, "Can't use 'super' in a class with no superclass.");
    }

    resolveLocal(&expr, expr.keyword);

    return std::nullptr_t{};
}

Object Resolver::visitThisExpr(const This& expr) {
    if(currentClass == ClassType::NONE) {
        Lox::error(expr.keyword, "Can't use 'this' outside of a class.");
        return std::nullptr_t{};
    }

    resolveLocal(&expr, expr.keyword);

    return std::nullptr_t{};
}

Object Resolver::visitUnaryExpr(const Unary& expr) {
    resolve(expr.right);
    
    return std::nullptr_t{};
}

Object Resolver::visitVariableExpr(const Variable& expr) {
    if (!scopes.empty() && scopes.top().contains(expr.name.lexeme) && scopes.top().at(expr.name.lexeme) == false) {
        Lox::error(expr.name, "Can't read local variable in its own initializer.");
    }

    resolveLocal(&expr, expr.name);

    return std::nullptr_t{};
}

void Resolver::resolve(const stmt_ptr& stmt) {
    stmt->accept(*this);
}

void Resolver::resolve(const expr_ptr& expr) {
    expr->accept(*this);
}

void Resolver::resolveFunction(const Function* stmt, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : stmt->params) {
      declare(param);
      define(param);
    }
    resolve(stmt->body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::beginScope() {
    scopes.push(std::unordered_map<std::string, bool>());
}

void Resolver::endScope() {
    scopes.pop();
}

void Resolver::declare(Token name) {
    if(scopes.empty()) return;
    std::unordered_map<std::string, bool> scope = scopes.top();

    if(scope.contains(name.lexeme)) {
        Lox::error(name, "Already a variable with this name in scope.");
    }

    scope[name.lexeme] = false;
}

void Resolver::define(Token name) {
    if(scopes.empty()) return;

    scopes.top()[name.lexeme] = true;
}

void Resolver::resolveLocal(const Expr* expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
      if (scopes.at(i).contains(name.lexeme)) {
        interpreter.resolve(*expr, scopes.size() - 1 - i);
        return;
      }
    }
}

