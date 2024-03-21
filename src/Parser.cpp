#include "../headers/Parser.hpp"
#include "../headers/Lox.hpp"

expr_ptr Parser::parse() {
    try {
        return expression();
    } catch(ParseError& error) {
        return NULL;
    }
}

expr_ptr Parser::expression() {
    return ternary();
    // return equality();
}

expr_ptr Parser::ternary() {

    expr_ptr expr = equality();

    while(match({QUESTION})) {
        Token op = previous();
        expr_ptr right = branch();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::branch() {
    
    expr_ptr expr = comma();

    while(match({COLON})) {
        Token op = previous();
        expr_ptr right = comma();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::comma() {

    expr_ptr expr = equality();

    while(match({COMMA})) {
        Token op = previous();
        expr_ptr right = equality();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

/**
 * lots of redundancy among equality(), comparison(), etc., find some
 * way to generalize and create a helper method among binary expression types
*/
expr_ptr Parser::equality() {

    expr_ptr expr = comparison();

    while(match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        expr_ptr right = comparison();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::comparison() {

    expr_ptr expr = term();

    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        expr_ptr right = term();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::term() {

    expr_ptr expr = factor();

    while(match({MINUS, PLUS})) {
        Token op = previous();
        expr_ptr right = factor();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::factor() {

    expr_ptr expr = unary();

    while(match({SLASH, STAR})) {
        Token op = previous();
        expr_ptr right = unary();

        expr_ptr temp(new Binary(std::move(expr), op, std::move(right)));
        expr = std::move(temp);
    }

    return expr;
}

expr_ptr Parser::unary() {
    if(match({BANG, MINUS})) {
        Token op = previous();
        expr_ptr right = unary();

        return expr_ptr(new Unary(op, std::move(right)));
    }

    return primary();
}

expr_ptr Parser::primary() {
    if(match({FALSE})) return expr_ptr(new Literal(false));
    if(match({TRUE})) return expr_ptr(new Literal(true));
    if(match({NIL})) return expr_ptr(new Literal(std::nullptr_t{}));

    if(match({NUMBER, STRING})) {
        return expr_ptr(new Literal(previous().literal));
    }

    if(match({LEFT_PAREN})) {
        expr_ptr expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        
        return expr_ptr(new Grouping(std::move(expr)));
    }

    throw error(peek(), "Expect Expression");
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