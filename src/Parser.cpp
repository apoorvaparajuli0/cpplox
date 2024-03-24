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
    //CHALLENGE(S) 6.1 & 6.2:
    // return ternary();
    return equality();
}

/**
 * CHALLENGE(S) 6.1 & 6.2:
 * Add Support for C-Style Ternary Operations
 * and the Comma operator
*/

// expr_ptr Parser::ternary() {
//     if(match({QUESTION})) {
//         try {
//             expr_ptr right_err = branch();
//         } catch(ParseError& err) {
//             throw error(peek(), "An Expression Must Precede the '?' Operator");
//         }
//     }

//     expr_ptr expr = parseBinaryExpression({QUESTION}, &Parser::equality , &Parser::branch );

//     return expr;
// }

// expr_ptr Parser::branch() {
//     if(match({COLON})) {
//         try {
//             expr_ptr right_err = comma();
//         } catch(ParseError& err) {
//             throw error(peek(), "An Expression Must Precede the ':' Operator");
//         }
//     }
    
//     expr_ptr expr = parseBinaryExpression({COLON}, &Parser::comma, &Parser::comma);

//     return expr;
// }

// expr_ptr Parser::comma() {
//     if(match({COMMA})) {
//         try {
//             expr_ptr right_err = equality();
//         } catch(ParseError& err) {
//             throw error(peek(), "An Expression Must Precede the ',' Operator");
//         }
//     }

//     expr_ptr expr = parseBinaryExpression({COMMA}, &Parser::equality, &Parser::equality);

//     return expr;
// }

expr_ptr Parser::equality() {
    /**
     * CHALLENGE 6.3:
     * Add Error Productions for Binary Operators
     * Without LH Operands
    */
    // if(match({BANG_EQUAL, EQUAL_EQUAL})) {
    //     try {
    //         expr_ptr right_err = comparison();
    //     } catch(ParseError& err) {
    //         throw error(peek(), "An Expression Must Precede an Equality Operator");
    //     }
    // }

    expr_ptr expr = parseBinaryExpression({BANG_EQUAL, EQUAL_EQUAL}, &Parser::comparison, &Parser::comparison);

    return expr;
}

expr_ptr Parser::comparison() {
    /**
     * CHALLENGE 6.3:
     * Add Error Productions for Binary Operators
     * Without LH Operands
    */
    // if(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    //     try {
    //         expr_ptr right_err = term();
    //     } catch(ParseError& err) {
    //         throw error(peek(), "An Expression Must Precede a Comparison Operator");
    //     }
    // }

    expr_ptr expr = parseBinaryExpression({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}, &Parser::term, &Parser::term);

    return expr;
}

/**
 * Didn't put error productions here since MINUS without a
 * preceding expression could be interpreted as a unary subtraction
 *  and PLUS not not having one is handled by the higher precedence
 *  error production 
*/
expr_ptr Parser::term() {
    /**
     * CHALLENGE 6.3:
     * Add Error Productions for Binary Operators
     * Without LH Operands
    */
    // if(match({PLUS})) {
    //     try {
    //         expr_ptr right_err = factor();
    //     } catch(ParseError& err) {
    //         throw error(peek(), "'+' Is Missing A Left Hand Operand");
    //     }
    // }
    expr_ptr expr = parseBinaryExpression({MINUS, PLUS}, &Parser::factor, &Parser::factor);
    return expr;
}

expr_ptr Parser::factor() {
    /**
     * CHALLENGE 6.3:
     * Add Error Productions for Binary Operators
     * Without LH Operands
    */
    // if(match({SLASH, STAR})) {
    //     try {
    //         expr_ptr right_err = unary();
    //     } catch(ParseError& err) {
    //         throw error(peek(), "An Expression Must Precede the Multiplication/Division Operators");
    //     }
    // }

    expr_ptr expr = parseBinaryExpression({SLASH, STAR}, &Parser::unary, &Parser::unary);

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