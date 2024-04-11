#include "../headers/Parser.hpp"
#include "../headers/Stmt.hpp"
#include "../headers/Expr.hpp"
#include "../headers/Lox.hpp"

std::vector<stmt_ptr> Parser::parse() {
    std::vector<stmt_ptr> statements;

    while(!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

expr_ptr Parser::expression() {
    return assignment();
}

stmt_ptr Parser::declaration() {
    try {
        if(match({CLASS})) return classDeclaration();
        if(match({FUN})) return function("function");
        if(match({VAR})) return varDeclaration();
        return statement();
    } catch(const ParseError& err) {
        synchronize();
        return std::nullptr_t{};
    }
}

stmt_ptr Parser::classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");

    expr_ptr superclass = std::nullptr_t{};
    if(match({LESS})) {
        consume(IDENTIFIER, "Expect superclass name.");
        superclass = expr_ptr(new Variable(previous()));
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<stmt_ptr> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return stmt_ptr(new Class(name, superclass, methods));
}

stmt_ptr Parser::statement() {
    if(match({FOR})) return forStatement();
    if(match({IF})) return ifStatement();
    if(match({PRINT})) return printStatement();
    if(match({RETURN})) return returnStatement();
    if(match({WHILE})) return whileStatement();
    if(match({LEFT_BRACE})) return stmt_ptr(new Block(block()));

    return expressionStatement();
}

stmt_ptr Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    stmt_ptr initializer;

    if(match({SEMICOLON})) {
        initializer = std::nullptr_t{};
    } else if(match({VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    expr_ptr condition = std::nullptr_t{};
    if(!check(SEMICOLON)) {
        condition = expression();
    }

    consume(SEMICOLON, "Expect ';' after loop condition");

    expr_ptr increment = std::nullptr_t{};
    if(!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    stmt_ptr body = statement();

    /**
     * Cannot initialize vector with unique ptr's because
     * initializer list elements are const, and subsequently
     * don't allow "moving"
     * 
     * Only copying is allowed into an init list, but unique_ptr's
     * can't be copied obviously
    */
    if(increment != std::nullptr_t{}) {
        std::vector<stmt_ptr> block_list; 
        block_list.push_back(std::move(body));
        block_list.push_back(stmt_ptr(new Expression(std::move(increment))));

        stmt_ptr temp(new Block(std::move(block_list)));
        body = std::move(temp);
    }

    if(condition == std::nullptr_t{}) {
        expr_ptr temp(new Literal(true)); 
        condition = std::move(temp);
    }

    stmt_ptr desugar(new While(std::move(condition), std::move(body)));
    body = std::move(desugar);

    if(initializer != std::nullptr_t{}) {
        std::vector<stmt_ptr> block_list; 
        block_list.push_back(std::move(initializer));
        block_list.push_back(std::move(body));

        stmt_ptr temp(new Block(std::move(block_list)));
        body = std::move(temp);
    }

    return body;
}

stmt_ptr Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    expr_ptr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    stmt_ptr thenBranch = statement();
    stmt_ptr elseBranch = std::nullptr_t{};
    if(match({ELSE})) {
        elseBranch = statement();
    }

    return stmt_ptr(new If(std::move(condition), std::move(thenBranch), std::move(elseBranch)));
}

stmt_ptr Parser::printStatement() {
    expr_ptr value = expression();
    consume(SEMICOLON, "Expect ';' after value");

    return stmt_ptr(new Print(std::move(value)));
}

stmt_ptr Parser::returnStatement() {
    Token keyword = previous();
    expr_ptr value = std::nullptr_t{};

    if(!check(SEMICOLON)) {
        value = expression();
    }

    consume(SEMICOLON, "Expect ';' after 'return' keyword");
    
    return stmt_ptr(new Return(keyword, std::move(value)));
}

stmt_ptr Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name");

    expr_ptr initializer = std::nullptr_t{};
    if(match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration");
    return stmt_ptr(new Var(name, std::move(initializer)));
}

stmt_ptr Parser::whileStatement() {

    consume(LEFT_PAREN, "Expect '(' Before Condition.");
    expr_ptr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' Before Condition.");
    stmt_ptr body = statement();
    
    return stmt_ptr(new While(std::move(condition), std::move(body)));
}

stmt_ptr Parser::expressionStatement() {
    expr_ptr expr = expression();
    consume(SEMICOLON, "Expect ';' after expression");
    return stmt_ptr(new Expression(std::move(expr)/*, false*/));
}

stmt_ptr Parser::function(std::string kind) {

    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");

    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }
        parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
      } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<stmt_ptr> body = block();

    return stmt_ptr(new Function(name, parameters, std::move(body)));
}

std::vector<stmt_ptr> Parser::block() {
    std::vector<stmt_ptr> statements;

    while(!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

expr_ptr Parser::assignment() {
    expr_ptr expr = log_or();

    if(match({EQUAL})) {
        Token equals = previous();
        expr_ptr value = assignment();

        if(dynamic_cast<Variable*>(expr.get()) != nullptr) {
            Token name = dynamic_cast<Variable*>(expr.get())->name;
            return expr_ptr(new Assign(name, std::move(value)));
        } else if(dynamic_cast<Get*>(expr.get()) != nullptr) {
            expr_ptr get(new Get(*(dynamic_cast<Get*>(expr.get()))));
            return expr_ptr(new Set(dynamic_cast<Get*>(get.get())->object, dynamic_cast<Get*>(get.get())->name, value));
        }

        error(equals, "Invalid Assignment Target.");
    }

    return expr;
}

expr_ptr Parser::log_or() {
    expr_ptr expr = log_and();

    while(match({OR})) {
        Token op = previous();
        expr_ptr right = log_and();
        expr_ptr temp(new Logical(std::move(expr), op, std::move(right)));

        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::log_and() {
    expr_ptr expr = equality();

    while(match({AND})) {
        Token op = previous();
        expr_ptr right = equality();
        expr_ptr temp(new Logical(std::move(expr), op, std::move(right)));

        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::equality() {
    expr_ptr expr = parseBinaryExpression({BANG_EQUAL, EQUAL_EQUAL}, &Parser::comparison, &Parser::comparison);
    return expr;
}

expr_ptr Parser::comparison() {
    expr_ptr expr = parseBinaryExpression({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}, &Parser::term, &Parser::term);
    return expr;
}

expr_ptr Parser::term() {
    expr_ptr expr = parseBinaryExpression({MINUS, PLUS}, &Parser::factor, &Parser::factor);
    return expr;
}

expr_ptr Parser::factor() {
    expr_ptr expr = parseBinaryExpression({SLASH, STAR}, &Parser::unary, &Parser::unary);
    return expr;
}

expr_ptr Parser::unary() {
    if(match({BANG, MINUS})) {
        Token op = previous();
        expr_ptr right = unary();

        return expr_ptr(new Unary(op, std::move(right)));
    }

    return call();
}

expr_ptr Parser::finishCall(expr_ptr& callee) {
    std::vector<expr_ptr> arguments;
    if (!check(RIGHT_PAREN)) {
      do {
        if(arguments.size() >= 255) error(peek(), "Can't have more than 255 arguments.");
        
        arguments.push_back(expression());
      } while (match({COMMA}));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return expr_ptr(new Call(std::move(callee), paren, std::move(arguments)));
}

expr_ptr Parser::call() {
    expr_ptr expr = primary();

    while (true) { 
      if (match({LEFT_PAREN})) {
        expr = finishCall(expr);
      } else if(match({DOT})) {
        Token name = consume(IDENTIFIER, "Expect Property Name After '.'.");
        expr_ptr temp(new Get(expr, name));
        expr = std::move(temp);
      } else {
        break;
      }
    }

    return expr;
}

expr_ptr Parser::primary() {
    if(match({FALSE})) return expr_ptr(new Literal(false));
    if(match({TRUE})) return expr_ptr(new Literal(true));
    if(match({NIL})) return expr_ptr(new Literal(std::nullptr_t{}));

    if(match({NUMBER, STRING})) {
        return expr_ptr(new Literal(previous().literal));
    }

    if(match({SUPER})) {
        Token keyword = previous();
        consume(DOT, "Expect '.' after 'super'");
        Token method = consume(IDENTIFIER, "Expect superclass method name.");

        return expr_ptr(new Super(keyword, method));
    }

    if(match({THIS})) return expr_ptr(new This(previous()));

    if(match({IDENTIFIER})) {
        return expr_ptr(new Variable(previous()));
    }

    if(match({LEFT_PAREN})) {
        expr_ptr expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        
        return expr_ptr(new Grouping(std::move(expr)));
    }

    throw error(peek(), "Expect Expression");
}

expr_ptr Parser::parseBinaryExpression(std::initializer_list<TokenType> tokenTypes, expr_ptr (Parser::* l_operand)(), expr_ptr (Parser::* r_operand)()) {
    expr_ptr expr = (this->*l_operand)();

    while(match(tokenTypes)) {
        Token op = previous();
        expr_ptr right = (this->*r_operand)();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for(TokenType type : types) {
        if(check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if(check(type)) return advance();
    throw error(peek(), message);
} 

Parser::ParseError Parser::error(Token token, std::string message) {
    Lox::error(token, message);
    return ParseError();
}

void Parser::synchronize() {
    advance();

    while(!isAtEnd()) {
        if(previous().type == SEMICOLON) return;

        switch(peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
            return;
        }
        advance();
    }
}

bool Parser::check(TokenType type) {
    if(isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if(!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOF_;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}