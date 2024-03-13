#include "iostream"
#include "fstream"
#include "vector"
#include "list"
#include "Scanner.hpp"
#include "Lox.hpp"


bool Lox::hadError = false;

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

    if(Lox::hadError) exit(65); 
}

void Lox::runPrompt() {
    for(;;) {
        printf("%s ", ">");
        std::string line;
        getline(std::cin, line);
        if(line.empty()) {
            break;
        }
        Lox::run(line);
        Lox::hadError = false;
    }
}

void Lox::run(std::string src) {
    Scanner scanner = Scanner(src);
    std::list<Token> tokens = scanner.scanTokens();

    for(Token token : tokens) {
        std::cout << token.toString();
    }
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    fprintf(stderr, "[line \" %d \"] Error %s : %s", line, where.c_str(), message.c_str());
    Lox::hadError = true;
}
