#include "Lox.hpp"
#include "iostream"
#include "fstream"
#include "vector"
#include "list"

bool Lox::hadError = false;

void Lox::runFile(std::string path) {
    std::ifstream fileBuffer(path, std::ios::in | std::ios::binary);
    std::vector<char> bytes;
    if(fileBuffer.good()) {
        char curr = NULL;
        while(fileBuffer >> curr) {
            if(curr != NULL) {
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
    Scanner scanner = new Scanner(src);
    std::list<Token> tokens scanner.scanTokens();

    for(Token token : tokens) {
        std::cout << token;
    }
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    fprintf(stderr, "[line \" %d \"] Error %s : %s", line, where, message);
    Lox::hadError = true;
}
