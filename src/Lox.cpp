#include "iostream"
#include "fstream"
#include "vector"
#include "../headers/Scanner.hpp"
#include "../headers/Lox.hpp"
#include "../headers/Parser.hpp"
#include "../headers/AstPrinter.hpp"

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
Interpreter Lox::interpreter = Interpreter();

void Lox::runFile(std::string path) {
    std::ifstream fileBuffer(path, std::ios::in | std::ios::binary);
    std::vector<char> bytes;
    if(fileBuffer.good()) {
        char curr = '\0';
        while(fileBuffer >> curr) {
            if(curr != '\0') {
                bytes.push_back(curr);
            }
        }
    }

    run(std::string(bytes.begin(), bytes.end()));

    if(hadError) exit(65); 
    if(hadRuntimeError) exit(70);
}

void Lox::runPrompt() {
    for(;;) {
        printf("%s ", ">");
        std::string currLine;
        getline(std::cin, currLine);
        if(std::cin.eof()) {
            break;
        }
        run(currLine);
        hadError = false;
    }
}

void Lox::run(std::string src) {
    Scanner scanner = Scanner(src);
    std::vector<Token> tokens = scanner.scanTokens();

    Parser parser = Parser(tokens);
    expr_ptr expression = parser.parse();

    if(hadError) return;
    
    interpreter.interpret(std::move(expression));
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    fprintf(stderr, "[line \" %d \"] Error %s : %s\n", line, where.c_str(), message.c_str());
    fprintf(stdout, "\n");
    hadError = true;
}

void Lox::error(Token token, std::string message) {
    if(token.type == EOF_) {
        report(token.line, " at end", message);
    } else {
      report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void Lox::runtimeError(RuntimeError error) {
    fprintf(stderr,"%s\n[line %d ]", error.what(), error.token.line);
    hadRuntimeError = true;
}
