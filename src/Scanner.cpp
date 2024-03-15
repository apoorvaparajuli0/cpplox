#include "Scanner.hpp"
#include "Lox.hpp"

std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    AND}, 
    {"class",  CLASS}, 
    {"else",   ELSE}, 
    {"false",  FALSE},
    {"for",    FOR}, 
    {"fun",    FUN}, 
    {"if",     IF}, 
    {"nil",    NIL}, 
    {"or",     OR}, 
    {"print",  PRINT}, 
    {"return", RETURN}, 
    {"super",  SUPER}, 
    {"this",   THIS},
    {"true",   TRUE}, 
    {"var",    VAR}, 
    {"while",  WHILE}
};

Scanner::Scanner(std::string &source) : 
                source(source)
{}

std::list<Token> Scanner::scanTokens() {
    while(!isAtEnd()) {
        start=current;
        scanToken();
    }
    Object obj = std::nullptr_t{};
    std::string str = "";
    Token toInsert = Token(EOF_, str, obj, line);

    tokens.push_back(toInsert);

    return tokens;
}

void Scanner::scanToken() {
    char c = advance();

    switch(c) {
        case '(': addToken(LEFT_PAREN);  break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE);  break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA);       break;
        case '.': addToken(DOT);         break;
        case '-': addToken(MINUS);       break;
        case '+': addToken(PLUS);        break;
        case ';': addToken(SEMICOLON);   break;
        case '*': addToken(STAR);        break; 
        case '!':
            addToken(match('=') ? BANG_EQUAL    : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL   : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL    : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            if(match('/')) {
                while(peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
            break;
        /*
        not 100% sure on why we're just ignoring 
        whitespace and carriage return instead of breaking
        */
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"': string(); break;
        default:
            if(isDigit(c)) {
                number();
            } else if(isAlpha(c)) {
                identifier();
            } else {
                Lox::error(line, "Unexpected Character");
            }
            break;
    }
}

void Scanner::identifier() {
    while(isAlphanumeric(peek())) advance();

    int offset = current - start;
    std::string text = source.substr(start, offset);
    TokenType type;

    //this is the cpp std::unordered_map equivalent of comparing with
    //null in Java's HashMap class
    if(keywords.find(text) == keywords.end()) { 
        type = IDENTIFIER;
    } else {
        type = keywords.at(text);
    }

    addToken(type);
}

void Scanner::number() {
    while(isDigit(peek())) advance();

    if(peek() == '.' && isDigit(peekNext())) {
        advance();

        while(isDigit(peek())) advance();
    }

    int offset = current - start; 
    Object numToken = std::stod(source.substr(start, offset));
    addToken(NUMBER, numToken);
}

void Scanner::string() {
    while(peek() != '"' && !isAtEnd()) {
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd()) {
        Lox::error(line, "Unterminated String.");
        return;
    }
    advance();

    /**
     * have to handle offset slightly differently here
     * due to the difference in how cpp handles substringing
    */
    int offset = (current - start) - 1;
    Object value = source.substr(start + 1, offset - 1);
    addToken(STRING, value);
}

bool Scanner::match(char expected) {
    if(isAtEnd()) return false;
    if(source.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if(isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext() {
    if(current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphanumeric(char c) {
    return (isDigit(c) || isAlpha(c));
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

char Scanner::advance() {
    return source.at(current++);
}

void Scanner::addToken(TokenType type) {
    Object obj = std::nullptr_t{};
    addToken(type, obj);
}

void Scanner::addToken(TokenType type, Object& literal) {
    int offset = current - start;
    std::string text = source.substr(start, offset);
    Token toInsert = Token(type, text, literal, line);
    tokens.push_back(toInsert);
}